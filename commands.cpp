#include "commands.h"

Commands::Commands(QObject *parent) : QObject(parent)
{

}

// Checks if the player is ready to start the game
void Commands::checkReady(std::string answer)
{
    if(answer == "PLAY")
    {
        clearConsole();
        startGame();
    } else if(answer == "HOWTOPLAY"){
        clearConsole();
        howToPlay();
        std::cin >> answer;
        checkReady(answer);
    } else {
        qInfo() << "Enter PLAY or HOWTOPLAY";
        std::cin >> answer;
        checkReady(answer);
    }
}

void Commands::howToPlay()
{
    qInfo() << "Objective: Track and sink all of the enemy ships before the enemy sinks yours";
    qInfo() << "There are three types of ships: Destroyer (2 grid ship), Cruiser (3 grid ship), and Battleship (4 grid ship).";
    qInfo() << "You and the enemy will take turns entering shots using the grid system. (Ex. A5, G7, F2)";
    qInfo() << "Enter PLAY when you are ready to start the game";
}

// Starts the game by randomly creating the map and displaying it
void Commands::startGame()
{
    insertBoats();
    checkExpansion();
    displayMap();
    beginShots();
}

// Randomly inserts the start for each boat
void Commands::insertBoats()
{
    //Enemy map
    for(int i = 0; i < 3; i++)
    {
                int x = QRandomGenerator::global()->bounded(8);
                int y = QRandomGenerator::global()->bounded(8);

                // Adds starter Destroyer Ship
                if(i == 0)
                {
                    EDX = x;
                    EDY = y;
                    mapEnemy[EDY][EDX] = 'D';
                }

                // Adds starter Cruiser Ship
                if(i == 1 && !(EDX == x && EDY == y))
                {
                    ECX = x;
                    ECY = y;
                    mapEnemy[ECY][ECX] = 'C';
                } else if(i == 1 && (EDX == x && EDY == y)){
                    i--;
                }

                //Adds starter Battleship
                if(i == 2 && !(EDX == x && EDY == y) && !(ECX == x && ECY == y)){
                    EBX = x;
                    EBY = y;
                    mapEnemy[EBY][EBX] = 'B';
                } else if(i == 2 && (EDX == x && EDY == y) && (ECX == x && ECY == y)){
                    i--;
                }
            }

    // Player map
    for(int i = 0; i < 3; i++)
    {
        int x = QRandomGenerator::global()->bounded(8);
        int y = QRandomGenerator::global()->bounded(8);

        // Adds starter Destroyer Ship
        if(i == 0)
        {
            PDX = x;
            PDY = y;
            mapPlayer[PDY][PDX] = 'D';
        }

        // Adds starter Cruiser Ship
        if(i == 1 && !(PDX == x && PDY == y))
        {
            PCX = x;
            PCY = y;
            mapPlayer[PCY][PCX] = 'C';
        } else if(i == 1 && (PDX == x && PDY == y)){
            i--;
        }

        //Adds starter Battleship
        if(i == 2 && !(PDX == x && PDY == y) && !(PCX == x && PCY == y))
        {
            PBX = x;
            PBY = y;
            mapPlayer[PBY][PBX] = 'B';
        } else if(i == 2 && (PDX == x && PDY == y) && (PCX == x && PCY == y)){
            i--;
        }
    }
}

// Checks the direction the boats can expand
void Commands::checkExpansion()
{
    // Enemy Map
    for(int i = 0; i < 3; i++)
    {
        // Expanding Destroyer
        if(i == 0)
        {
            while(eDestroyer)
            {
                int x = QRandomGenerator::global()->bounded(4);
                if(x == 0) // North
                {
                    if(mapEnemy[EDY - 1][EDX] == '*' && EDY != 0){
                        expandBoat("Enemy", "Destroyer", "North");
                    }

                } else if(x == 1){ // East
                    if(mapEnemy[EDY][EDX + 1] == '*' && EDX != 7){
                        expandBoat("Enemy", "Destroyer", "East");
                    }
                } else if(x == 2){ // South
                    if(mapEnemy[EDY + 1][EDX] == '*' && EDY != 7){
                        expandBoat("Enemy", "Destroyer", "South");
                    }
                } else if(x == 3){ // West
                    if(mapEnemy[EDY][EDX - 1] == '*' && EDX != 0){
                        expandBoat("Enemy", "Destroyer", "West");
                    }
                }
            }
        } else if(i == 1){
            while(eCruiser)
            {
                int y = QRandomGenerator::global()->bounded(4);
                if(y == 0) // North
                {
                    if(mapEnemy[ECY - 1][ECX] == '*' && mapEnemy[ECY - 2][ECX] == '*' && ECY > 1){
                        expandBoat("Enemy", "Cruiser", "North");
                    }

                } else if(y == 1){ // East
                    if(mapEnemy[ECY][ECX + 1] == '*' && mapEnemy[ECY][ECX + 2] == '*' && ECX < 6){
                        expandBoat("Enemy", "Cruiser", "East");
                    }
                } else if(y == 2){ // South
                    if(mapEnemy[ECY + 1][ECX] == '*' && mapEnemy[ECY + 2][ECX] == '*' && ECY < 6){
                        expandBoat("Enemy", "Cruiser", "South");
                    }
                } else if(y == 3){ // West
                    if(mapEnemy[ECY][ECX - 1] == '*' && mapEnemy[ECY][ECX - 2] == '*' && ECX > 1){
                        expandBoat("Enemy", "Cruiser", "West");
                    }
                }
            }
        } else if(i == 2){
            while(eBattleship)
            {
                int z = QRandomGenerator::global()->bounded(4);
                if(z == 0) // North
                {
                    if(mapEnemy[EBY - 1][EBX] == '*' && mapEnemy[EBY - 2][EBX] == '*' && mapEnemy[EBY - 3][EBX] == '*' && EBY > 2){
                        expandBoat("Enemy", "Battleship", "North");
                    }

                } else if(z == 1){ // East
                    if(mapEnemy[EBY][EBX + 1] == '*' && mapEnemy[EBY][EBX + 2] == '*' && mapEnemy[EBY][EBX + 3] == '*' && EBX < 5){
                        expandBoat("Enemy", "Battleship", "East");
                    }
                } else if(z == 2){ // South
                    if(mapEnemy[EBY + 1][EBX] == '*' && mapEnemy[EBY + 2][EBX] == '*' && mapEnemy[EBY + 3][EBX] == '*' && EBY < 5){
                        expandBoat("Enemy", "Battleship", "South");
                    }
                } else if(z == 3){ // West
                    if(mapEnemy[EBY][EBX - 1] == '*' && mapEnemy[EBY][EBX - 2] == '*' && mapEnemy[EBY][EBX - 3] == '*' && EBX > 2){
                        expandBoat("Enemy", "Battleship", "West");
                    }
                }
            }
        }
    }

    // Player Map
    for(int i = 0; i < 3; i++)
    {
        // Expanding Destroyer
        if(i == 0)
        {
            while(pDestroyer)
            {
                int x = QRandomGenerator::global()->bounded(4);
                if(x == 0) // North
                {
                    if(mapPlayer[PDY - 1][PDX] == '*' && PDY != 0){
                        expandBoat("Player", "Destroyer", "North");
                    }

                } else if(x == 1){ // East
                    if(mapPlayer[PDY][PDX + 1] == '*' && PDX != 7){
                        expandBoat("Player", "Destroyer", "East");
                    }
                } else if(x == 2){ // South
                    if(mapPlayer[PDY + 1][PDX] == '*' && PDY != 7){
                        expandBoat("Player", "Destroyer", "South");
                    }
                } else if(x == 3){ // West
                    if(mapPlayer[PDY][PDX - 1] == '*' && PDX != 0){
                        expandBoat("Player", "Destroyer", "West");
                    }
                }
            }
        } else if(i == 1){
            while(pCruiser)
            {
                int y = QRandomGenerator::global()->bounded(4);
                if(y == 0) // North
                {
                    if(mapPlayer[PCY - 1][PCX] == '*' && mapPlayer[PCY - 2][PCX] == '*' && PCY > 1){
                        expandBoat("Player", "Cruiser", "North");
                    }

                } else if(y == 1){ // East
                    if(mapPlayer[PCY][PCX + 1] == '*' && mapPlayer[PCY][PCX + 2] == '*' && PCX < 6){
                        expandBoat("Player", "Cruiser", "East");
                    }
                } else if(y == 2){ // South
                    if(mapPlayer[PCY + 1][PCX] == '*' && mapPlayer[PCY + 2][PCX] == '*' && PCY < 6){
                        expandBoat("Player", "Cruiser", "South");
                    }
                } else if(y == 3){ // West
                    if(mapPlayer[PCY][PCX - 1] == '*' && mapPlayer[PCY][PCX - 2] == '*' && PCX > 1){
                        expandBoat("Player", "Cruiser", "West");
                    }
                }
            }
        } else if(i == 2){
            while(pBattleship)
            {
                int z = QRandomGenerator::global()->bounded(4);
                if(z == 0) // North
                {
                    if(mapPlayer[PBY - 1][PBX] == '*' && mapPlayer[PBY - 2][PBX] == '*' && mapPlayer[PBY - 3][PBX] == '*' && PBY > 2){
                        expandBoat("Player", "Battleship", "North");
                    }

                } else if(z == 1){ // East
                    if(mapPlayer[PBY][PBX + 1] == '*' && mapPlayer[PBY][PBX + 2] == '*' && mapPlayer[PBY][PBX + 3] == '*' && PBX < 5){
                        expandBoat("Player", "Battleship", "East");
                    }
                } else if(z == 2){ // South
                    if(mapPlayer[PBY + 1][PBX] == '*' && mapPlayer[PBY + 2][PBX] == '*' && mapPlayer[PBY + 3][PBX] == '*' && PBY < 5){
                        expandBoat("Player", "Battleship", "South");
                    }
                } else if(z == 3){ // West
                    if(mapPlayer[PBY][PBX - 1] == '*' && mapPlayer[PBY][PBX - 2] == '*' && mapPlayer[PBY][PBX - 3] == '*' && PBX > 2){
                        expandBoat("Player", "Battleship", "West");
                    }
                }
            }
        }
    }
}

// Expands the boats in the set directions
void Commands::expandBoat(std::string player, std::string boat, std::string direction)
{
    if(player == "Enemy"){
        if(boat == "Destroyer"){
            if(direction == "North"){
                mapEnemy[EDY - 1][EDX] = 'D';
                EDY2 = EDY - 1;
                EDX2 = EDX;
            } else if(direction == "East"){
                mapEnemy[EDY][EDX + 1] = 'D';
                EDY2 = EDY;
                EDX2 = EDX + 1;
            } else if(direction == "South"){
                mapEnemy[EDY + 1][EDX] = 'D';
                EDY2 = EDY + 1;
                EDX2 = EDX;
            } else if(direction == "West"){
                mapEnemy[EDY][EDX - 1] = 'D';
                EDY2 = EDY;
                EDX2 = EDX - 1;
            }
            eDestroyer = false;
        } else if(boat == "Cruiser"){
            if(direction == "North"){
                mapEnemy[ECY - 1][ECX] = 'C';
                mapEnemy[ECY - 2][ECX] = 'C';
                ECY2 = ECY - 1;
                ECX2 = ECX;
                ECY3 = ECY - 2;
                ECX3 = ECX;
            } else if(direction == "East"){
                mapEnemy[ECY][ECX + 1] = 'C';
                mapEnemy[ECY][ECX + 2] = 'C';
                ECY2 = ECY;
                ECX2 = ECX + 1;
                ECY3 = ECY;
                ECX3 = ECX + 2;
            } else if(direction == "South"){
                mapEnemy[ECY + 1][ECX] = 'C';
                mapEnemy[ECY + 2][ECX] = 'C';
                ECY2 = ECY + 1;
                ECX2 = ECX;
                ECY3 = ECY + 2;
                ECX3 = ECX;
            } else if(direction == "West"){
                mapEnemy[ECY][ECX - 1] = 'C';
                mapEnemy[ECY][ECX - 2] = 'C';
                ECY2 = ECY;
                ECX2 = ECX - 1;
                ECY3 = ECY;
                ECX3 = ECX - 2;
            }
            eCruiser = false;
        } else if(boat == "Battleship"){
            if(direction == "North"){
                mapEnemy[EBY - 1][EBX] = 'B';
                mapEnemy[EBY - 2][EBX] = 'B';
                mapEnemy[EBY - 3][EBX] = 'B';
                EBY2 = EBY - 1;
                EBX2 = EBX;
                EBY3 = EBY - 2;
                EBX3 = EBX;
                EBY4 = EBY - 3;
                EBX4 = EBX;
            } else if(direction == "East"){
                mapEnemy[EBY][EBX + 1] = 'B';
                mapEnemy[EBY][EBX + 2] = 'B';
                mapEnemy[EBY][EBX + 3] = 'B';
                EBY2 = EBY;
                EBX2 = EBX + 1;
                EBY3 = EBY;
                EBX3 = EBX + 2;
                EBY4 = EBY;
                EBX4 = EBX + 3;
            } else if(direction == "South"){
                mapEnemy[EBY + 1][EBX] = 'B';
                mapEnemy[EBY + 2][EBX] = 'B';
                mapEnemy[EBY + 3][EBX] = 'B';
                EBY2 = EBY + 1;
                EBX2 = EBX;
                EBY3 = EBY + 2;
                EBX3 = EBX;
                EBY4 = EBY + 3;
                EBX4 = EBX;
            } else if(direction == "West"){
                mapEnemy[EBY][EBX - 1] = 'B';
                mapEnemy[EBY][EBX - 2] = 'B';
                mapEnemy[EBY][EBX - 3] = 'B';
                EBY2 = EBY;
                EBX2 = EBX - 1;
                EBY3 = EBY;
                EBX3 = EBX - 2;
                EBY4 = EBY;
                EBX4 = EBX - 3;
            }
            eBattleship = false;
        }
    } else if(player == "Player"){
        if(boat == "Destroyer"){
            if(direction == "North"){
                mapPlayer[PDY - 1][PDX] = 'D';
                PDY2 = PDY - 1;
                PDX2 = PDX;
            } else if(direction == "East"){
                mapPlayer[PDY][PDX + 1] = 'D';
                PDY2 = PDY;
                PDX2 = PDX + 1;
            } else if(direction == "South"){
                mapPlayer[PDY + 1][PDX] = 'D';
                PDY2 = PDY + 1;
                PDX2 = PDX;
            } else if(direction == "West"){
                mapPlayer[PDY][PDX - 1] = 'D';
                PDY2 = PDY;
                PDX2 = PDX - 1;
            }
            pDestroyer = false;
        } else if(boat == "Cruiser"){
            if(direction == "North"){
                mapPlayer[PCY - 1][PCX] = 'C';
                mapPlayer[PCY - 2][PCX] = 'C';
                PCY2 = PCY - 1;
                PCX2 = PCX;
                PCY3 = PCY - 2;
                PCX3 = PCX;
            } else if(direction == "East"){
                mapPlayer[PCY][PCX + 1] = 'C';
                mapPlayer[PCY][PCX + 2] = 'C';
                PCY2 = PCY;
                PCX2 = PCX + 1;
                PCY3 = PCY;
                PCX3 = PCX + 2;
            } else if(direction == "South"){
                mapPlayer[PCY + 1][PCX] = 'C';
                mapPlayer[PCY + 2][PCX] = 'C';
                PCY2 = PCY + 1;
                PCX2 = PCX;
                PCY3 = PCY + 2;
                PCX3 = PCX;
            } else if(direction == "West"){
                mapPlayer[PCY][PCX - 1] = 'C';
                mapPlayer[PCY][PCX - 2] = 'C';
                PCY2 = PCY;
                PCX2 = PCX - 1;
                PCY3 = PCY;
                PCX3 = PCX - 2;
            }
            pCruiser = false;
        } else if(boat == "Battleship"){
            if(direction == "North"){
                mapPlayer[PBY - 1][PBX] = 'B';
                mapPlayer[PBY - 2][PBX] = 'B';
                mapPlayer[PBY - 3][PBX] = 'B';
                PBY2 = PBY - 1;
                PBX2 = PBX;
                PBY3 = PBY - 2;
                PBX3 = PBX;
                PBY4 = PBY - 3;
                PBX4 = PBX;
            } else if(direction == "East"){
                mapPlayer[PBY][PBX + 1] = 'B';
                mapPlayer[PBY][PBX + 2] = 'B';
                mapPlayer[PBY][PBX + 3] = 'B';
                PBY2 = PBY;
                PBX2 = PBX + 1;
                PBY3 = PBY;
                PBX3 = PBX + 2;
                PBY4 = PBY;
                PBX4 = PBX + 3;
            } else if(direction == "South"){
                mapPlayer[PBY + 1][PBX] = 'B';
                mapPlayer[PBY + 2][PBX] = 'B';
                mapPlayer[PBY + 3][PBX] = 'B';
                PBY2 = PBY + 1;
                PBX2 = PBX;
                PBY3 = PBY + 2;
                PBX3 = PBX;
                PBY4 = PBY + 3;
                PBX4 = PBX;
            } else if(direction == "West"){
                mapPlayer[PBY][PBX - 1] = 'B';
                mapPlayer[PBY][PBX - 2] = 'B';
                mapPlayer[PBY][PBX - 3] = 'B';
                PBY2 = PBY;
                PBX2 = PBX - 1;
                PBY3 = PBY;
                PBX3 = PBX - 2;
                PBY4 = PBY;
                PBX4 = PBX - 3;
            }
            pBattleship = false;
        }
    }
}

// Displays the maps on terminal for the player
void Commands::displayMap()
{
    qInfo() << "ENEMY MAP";
    qInfo() << underlineON << "|1|2|3|4|5|6|7|8|" << underlineOFF;

    //Printing out the Enemy Map
    for(int i = 0; i < 8; i++)
    {
        if(i == 0) std::cout << "A|";
        if(i == 1) std::cout << "B|";
        if(i == 2) std::cout << "C|";
        if(i == 3) std::cout << "D|";
        if(i == 4) std::cout << "E|";
        if(i == 5) std::cout << "F|";
        if(i == 6) std::cout << "G|";
        if(i == 7) std::cout << "H|";
        for(int j = 0; j < 8; j++)
        {
            if(j != 7) std::cout << shownEnemy[i][j] << " ";
            if(j == 7) std::cout << shownEnemy[i][j] << "|";
        }
        std::cout << std::endl;
    }

    //Divider inbetween maps
    for(int i = 0; i < 10; i++) i != 9 ? std::cout << "@ " :  std::cout << "@" << std::endl;
    //Printing out the Players map
    for(int i = 0; i < 8; i++)
    {
        if(i == 0) std::cout << "A|";
        if(i == 1) std::cout << "B|";
        if(i == 2) std::cout << "C|";
        if(i == 3) std::cout << "D|";
        if(i == 4) std::cout << "E|";
        if(i == 5) std::cout << "F|";
        if(i == 6) std::cout << "G|";
        if(i == 7) std::cout << underlineON << "H|" << underlineOFF;
        for(int j = 0; j < 8; j++)
        {
            if(j != 7 && i != 7) std::cout << mapPlayer[i][j] << " ";
            if(j != 7 && i == 7) std::cout << underlineON << mapPlayer[i][j] << " " << underlineOFF;
            if(j == 7 && i != 7) std::cout << mapPlayer[i][j] << "|";
            if(j == 7 && i == 7) std::cout << underlineON << mapPlayer[i][j] << "| " << underlineOFF;
        }
        std::cout << std::endl;
    }
    qInfo() << " |1|2|3|4|5|6|7|8|";
    qInfo() << "YOUR MAP";
}

// Clears the terminal
void Commands::clearConsole()
{
    QProcess::execute("cmd /c cls");
}

// Allows the game to start taking shots
void Commands::beginShots()
{
    while(!gameOver){
        takeShot();
        enemyShot();
        clearConsole();
        displayMap();
        checkPlayerShot();
        checkEnemyShot();
        checkWin();
    }
}

// Allows the player to fire
void Commands::takeShot()
{
    std::cout << "Enter a spot to fire (Ex. A6): ";
    std::cin >> firing;
    while(!verifyShot("Player", firing))
    {
        std::cout << "Please enter a valid spot (Ex. D2): ";
        std::cin >> firing;
    }
}

// Randomly picks where the enemy will shoot unless enemy got hit last time, then it will find the remaining parts of the ship
std::string Commands::pickEnemyShot()
{
    std::string position;
    int x = QRandomGenerator::global()->bounded(8);
    int y = QRandomGenerator::global()->bounded(8);

    if(EHD)
    {
        int random = QRandomGenerator::global()->bounded(4);
        if(PDHIT)
        {
            if(random == 0){
                x = PDX + 1;
                y = PDY;
            } else if(random == 1){
                x = PDX;
                y = PDY + 1;
            } else if(random == 2){
                x = PDX - 1;
                y = PDY;
            } else if(random == 3){
                x = PDX;
                y = PDY - 1;
            }
        } else if(PDHIT2){
            if(random == 0){
                x = PDX2 + 1;
                y = PDY2;
            } else if(random == 1){
                x = PDX2;
                y = PDY2 + 1;
            } else if(random == 2){
                x = PDX2 - 1;
                y = PDY2;
            } else if(random == 3){
                x = PDX2;
                y = PDY2 - 1;
            }
        }
    } else if(EHC) {
        if(PCC == 1)
        {
            int random = QRandomGenerator::global()->bounded(4);
            if(PCHIT)
            {
                cog = "PCHIT";
                if(random == 0){
                    x = PCX + 1;
                    y = PCY;
                    cdir = "East";
                } else if(random == 1){
                    x = PCX;
                    y = PCY + 1;
                    cdir = "North";
                } else if(random == 2){
                    x = PCX - 1;
                    y = PCY;
                    cdir = "West";
                } else if(random == 3){
                    x = PCX;
                    y = PCY - 1;
                    cdir = "South";
                }
            } else if(PCHIT2){
                cog = "PCHIT2";
                if(random == 0){
                    x = PCX2 + 1;
                    y = PCY2;
                    cdir = "East";
                } else if(random == 1){
                    x = PCX2;
                    y = PCY2 + 1;
                    cdir = "North";
                } else if(random == 2){
                    x = PCX2 - 1;
                    y = PCY2;
                    cdir = "West";
                } else if(random == 3){
                    x = PCX2;
                    y = PCY2 - 1;
                    cdir = "South";
                }
            } else if(PCHIT3){
                cog = "PCHIT3";
                if(random == 0){
                    x = PCX3 + 1;
                    y = PCY3;
                    cdir = "East";
                } else if(random == 1){
                    x = PCX3;
                    y = PCY3 + 1;
                    cdir = "North";
                } else if(random == 2){
                    x = PCX3 - 1;
                    y = PCY3;
                    cdir = "West";
                } else if(random == 3){
                    x = PCX3;
                    y = PCY3 - 1;
                    cdir = "South";
                }
            }
        } else if(PCC == 2){
            //CC = 3;
            CC--;
            if(cdir == "East"){
                if(cog == "PCHIT"){
                   x = PCX + CC;
                   y = PCY;
                } else if(cog == "PCHIT2"){
                    x = PCX2 + CC;
                    y = PCY2;
                } else if(cog == "PCHIT3"){
                    x = PCX3 + CC;
                    y = PCY3;
                }
                cdir = "West";
            } else if(cdir == "North"){
                if(cog == "PCHIT"){
                   x = PCX;
                   y = PCY + CC;
                } else if(cog == "PCHIT2"){
                    x = PCX2;
                    y = PCY2 + CC;
                } else if(cog == "PCHIT3"){
                    x = PCX3;
                    y = PCY3 + CC;
                }
                cdir = "South";
            } else if(cdir == "West"){
                if(cog == "PCHIT"){
                   x = PCX - CC;
                   y = PCY;
                } else if(cog == "PCHIT2"){
                    x = PCX2 - CC;
                    y = PCY2;
                } else if(cog == "PCHIT3"){
                    x = PCX3 - CC;
                    y = PCY3;
                }
                cdir = "East";
            } else if(cdir == "South"){
                if(cog == "PCHIT"){
                   x = PCX;
                   y = PCY - CC;
                } else if(cog == "PCHIT2"){
                    x = PCX2;
                    y = PCY2 - CC;
                } else if(cog == "PCHIT3"){
                    x = PCX3;
                    y = PCY3 - CC;
                }
                cdir = "North";
            }
        }
    } else if(EHB){
        if(PBC == 1)
        {
            int random = QRandomGenerator::global()->bounded(4);
            if(PBHIT)
            {
                bog = "PBHIT";
                if(random == 0){
                    x = PBX + 1;
                    y = PBY;
                    bdir = "East";
                } else if(random == 1){
                    x = PBX;
                    y = PBY + 1;
                    bdir = "North";
                } else if(random == 2){
                    x = PBX - 1;
                    y = PBY;
                    bdir = "West";
                } else if(random == 3){
                    x = PBX;
                    y = PBY - 1;
                    bdir = "South";
                }
            } else if(PBHIT2){
                bog = "PBHIT2";
                if(random == 0){
                    x = PBX2 + 1;
                    y = PBY2;
                    bdir = "East";
                } else if(random == 1){
                    x = PBX2;
                    y = PBY2 + 1;
                    bdir = "North";
                } else if(random == 2){
                    x = PBX2 - 1;
                    y = PBY2;
                    bdir = "West";
                } else if(random == 3){
                    x = PBX2;
                    y = PBY2 - 1;
                    bdir = "South";
                }
            } else if(PBHIT3){
                bog = "PBHIT3";
                if(random == 0){
                    x = PBX3 + 1;
                    y = PBY3;
                    bdir = "East";
                } else if(random == 1){
                    x = PBX3;
                    y = PBY3 + 1;
                    bdir = "North";
                } else if(random == 2){
                    x = PBX3 - 1;
                    y = PBY3;
                    bdir = "West";
                } else if(random == 3){
                    x = PBX3;
                    y = PBY3 - 1;
                    bdir = "South";
                }
            } else if(PBHIT4){
                bog = "PBHIT4";
                if(random == 0){
                    x = PBX4 + 1;
                    y = PBY4;
                    bdir = "East";
                } else if(random == 1){
                    x = PBX4;
                    y = PBY4 + 1;
                    bdir = "North";
                } else if(random == 2){
                    x = PBX4 - 1;
                    y = PBY4;
                    bdir = "West";
                } else if(random == 3){
                    x = PBX4;
                    y = PBY4 - 1;
                    bdir = "South";
                }
            }
        } else if(PBC == 2){
            BC--; // now 2
            if(bdir == "East"){
                if(bog == "PBHIT"){
                    x = PBX + BC;
                    y = PBY;
                } else if(bog == "PBHIT2"){
                    x = PBX2 + BC;
                    y = PBY2;
                } else if(bog == "PBHIT3"){
                    x = PBX3 + BC;
                    y = PBY3;
                } else if(bog == "PBHIT4"){
                    x = PBX4 + BC;
                    y = PBY4;
                }
                bdir = "West";
            } else if(bdir == "North"){
                if(bog == "PBHIT"){
                    x = PBX;
                    y = PBY + BC;
                } else if(bog == "PBHIT2"){
                    x = PBX2;
                    y = PBY2 + BC;
                } else if(bog == "PBHIT3"){
                    x = PBX3;
                    y = PBY3 + BC;
                } else if(bog == "PBHIT4"){
                    x = PBX4;
                    y = PBY4 + BC;
                }
                bdir = "South";
            } else if(bdir == "West"){
                if(bog == "PBHIT"){
                    x = PBX - BC;
                    y = PBY;
                } else if(bog == "PBHIT2"){
                    x = PBX2 - BC;
                    y = PBY2;
                } else if(bog == "PBHIT3"){
                    x = PBX3 - BC;
                    y = PBY3;
                } else if(bog == "PBHIT4"){
                    x = PBX4 - BC;
                    y = PBY4;
                }
                bdir = "East";
            } else if(bdir == "South"){
                if(bog == "PBHIT"){
                    x = PBX;
                    y = PBY - BC;
                } else if(bog == "PBHIT2"){
                    x = PBX2;
                    y = PBY2 - BC;
                } else if(bog == "PBHIT3"){
                    x = PBX3;
                    y = PBY3 - BC;
                } else if(bog == "PBHIT4"){
                    x = PBX4;
                    y = PBY4 - BC;
                }
                bdir = "North";
            }
        } else if(PBC == 3) {
            BC--; // 1
            if(bdir == "East"){
                if(bog == "PBHIT"){
                    x = PBX + BC;
                    y = PBY;
                    BC = 4;
                } else if(bog == "PBHIT2"){
                    x = PBX2 + BC;
                    y = PBY2;
                } else if(bog == "PBHIT3"){
                    x = PBX3 + BC;
                    y = PBY3;
                } else if(bog == "PBHIT4"){
                    x = PBX4 + BC;
                    y = PBY4;
                    BC = 4;
                }
                bdir = "West";
            } else if(bdir == "North"){
                if(bog == "PBHIT"){
                    x = PBX;
                    y = PBY + BC;
                    BC = 4;
                } else if(bog == "PBHIT2"){
                    x = PBX2;
                    y = PBY2 + BC;
                } else if(bog == "PBHIT3"){
                    x = PBX3;
                    y = PBY3 + BC;
                } else if(bog == "PBHIT4"){
                    x = PBX4;
                    y = PBY4 + BC;
                    BC = 4;
                }
                bdir = "South";
            } else if(bdir == "West"){
                if(bog == "PBHIT"){
                    x = PBX - BC;
                    y = PBY;
                    BC = 4;
                } else if(bog == "PBHIT2"){
                    x = PBX2 - BC;
                    y = PBY2;
                } else if(bog == "PBHIT3"){
                    x = PBX3 - BC;
                    y = PBY3;
                } else if(bog == "PBHIT4"){
                    x = PBX4 - BC;
                    y = PBY4;
                    BC = 4;
                }
                bdir = "East";
            } else if(bdir == "South"){
                if(bog == "PBHIT"){
                    x = PBX;
                    y = PBY - BC;
                    BC = 4;
                } else if(bog == "PBHIT2"){
                    x = PBX2;
                    y = PBY2 - BC;
                } else if(bog == "PBHIT3"){
                    x = PBX3;
                    y = PBY3 - BC;
                } else if(bog == "PBHIT4"){
                    x = PBX4;
                    y = PBY4 - BC;
                    BC = 4;
                }
                bdir = "North";
            }
        }
    }

    if(y == 0){
        if(x == 0){
            position = "A1";
        } else if(x == 1) {
            position = "A2";
        } else if(x == 2) {
            position = "A3";
        } else if(x == 3) {
            position = "A4";
        } else if(x == 4) {
            position = "A5";
        } else if(x == 5) {
            position = "A6";
        } else if(x == 6) {
            position = "A7";
        } else if(x == 7) {
            position = "A8";
        }
    } else if(y == 1) {
        if(x == 0){
            position = "B1";
        } else if(x == 1) {
            position = "B2";
        } else if(x == 2) {
            position = "B3";
        } else if(x == 3) {
            position = "B4";
        } else if(x == 4) {
            position = "B5";
        } else if(x == 5) {
            position = "B6";
        } else if(x == 6) {
            position = "B7";
        } else if(x == 7) {
            position = "B8";
        }
    } else if(y == 2) {
        if(x == 0){
            position = "C1";
        } else if(x == 1) {
            position = "C2";
        } else if(x == 2) {
            position = "C3";
        } else if(x == 3) {
            position = "C4";
        } else if(x == 4) {
            position = "C5";
        } else if(x == 5) {
            position = "C6";
        } else if(x == 6) {
            position = "C7";
        } else if(x == 7) {
            position = "C8";
        }
    } else if(y == 3) {
        if(x == 0){
            position = "D1";
        } else if(x == 1) {
            position = "D2";
        } else if(x == 2) {
            position = "D3";
        } else if(x == 3) {
            position = "D4";
        } else if(x == 4) {
            position = "D5";
        } else if(x == 5) {
            position = "D6";
        } else if(x == 6) {
            position = "D7";
        } else if(x == 7) {
            position = "D8";
        }
    } else if(y == 4) {
        if(x == 0){
            position = "E1";
        } else if(x == 1) {
            position = "E2";
        } else if(x == 2) {
            position = "E3";
        } else if(x == 3) {
            position = "E4";
        } else if(x == 4) {
            position = "E5";
        } else if(x == 5) {
            position = "E6";
        } else if(x == 6) {
            position = "E7";
        } else if(x == 7) {
            position = "E8";
        }
    } else if(y == 5) {
        if(x == 0){
            position = "F1";
        } else if(x == 1) {
            position = "F2";
        } else if(x == 2) {
            position = "F3";
        } else if(x == 3) {
            position = "F4";
        } else if(x == 4) {
            position = "F5";
        } else if(x == 5) {
            position = "F6";
        } else if(x == 6) {
            position = "F7";
        } else if(x == 7) {
            position = "F8";
        }
    } else if(y == 6) {
        if(x == 0){
            position = "G1";
        } else if(x == 1) {
            position = "G2";
        } else if(x == 2) {
            position = "G3";
        } else if(x == 3) {
            position = "G4";
        } else if(x == 4) {
            position = "G5";
        } else if(x == 5) {
            position = "G6";
        } else if(x == 6) {
            position = "G7";
        } else if(x == 7) {
            position = "G8";
        }
    } else if(y == 7) {
        if(x == 0){
            position = "H1";
        } else if(x == 1) {
            position = "H2";
        } else if(x == 2) {
            position = "H3";
        } else if(x == 3) {
            position = "H4";
        } else if(x == 4) {
            position = "H5";
        } else if(x == 5) {
            position = "H6";
        } else if(x == 6) {
            position = "H7";
        } else if(x == 7) {
            position = "H8";
        }
    }

    enemyPastShot = position;
    return position;
}

// Makes the enemy fire
void Commands::enemyShot()
{
    std::string position = pickEnemyShot();
    while(!verifyShot("Enemy", position))
    {
        position = pickEnemyShot();
    }
}

// Verifies if player or enemy are firing at a valid location
bool Commands::verifyShot(std::string player, std::string shot)
{
    if(shot.size() != 2) return false;
    if(shot.at(0) < 65 || shot.at(0) > 72) return false;
    if(shot.at(1) < 49 || shot.at(1) > 56) return false;

    int y = shot.at(0) - 65; // A = 65
    int x = shot.at(1) - 49; // 1 = 49

    if(player == "Player")
    {
        playerPastShot = shot;
        if(mapEnemy[y][x] == '*'){
            shownEnemy[y][x] = 'O';
            mapEnemy[y][x] = 'O';
            return true;
        } else if(mapEnemy[y][x] == 'D' || mapEnemy[y][x] == 'C' || mapEnemy[y][x] == 'B') {
            shownEnemy[y][x] = 'X';
            mapEnemy[y][x] = 'X';
            return true;
        }
    } else if("Enemy"){
        if(mapPlayer[y][x] == '*'){
            mapPlayer[y][x] = 'O';
            return true;
        } else if(mapPlayer[y][x] == 'D' || mapPlayer[y][x] == 'C' || mapPlayer[y][x] == 'B') {
            mapPlayer[y][x] = 'X';
            return true;
        }
    }

    return false;
}

// Displays if player shot hit, sunk, or missed the ships
void Commands::checkPlayerShot()
{
    int count = 0;
    if((mapEnemy[EDY][EDX] == 'X' && !EDHIT) || (mapEnemy[EDY2][EDX2] == 'X' && !EDHIT2))
    {
        if(mapEnemy[EDY][EDX] == 'X') EDHIT = true;
        if(mapEnemy[EDY2][EDX2] == 'X') EDHIT2 = true;
        std::cout << "Enemy Destroyer has been hit at " << firing << "!" << std::endl;
        EDC++;
        if(EDC == 2) qInfo() << "Enemy Destroyer has sunk!";
    } else {
        count++;
    }
    if((mapEnemy[ECY][ECX] == 'X' && !ECHIT) || (mapEnemy[ECY2][ECX2] == 'X' && !ECHIT2) || (mapEnemy[ECY3][ECX3] == 'X' && !ECHIT3))
    {
        if(mapEnemy[ECY][ECX] == 'X') ECHIT = true;
        if(mapEnemy[ECY2][ECX2] == 'X') ECHIT2 = true;
        if(mapEnemy[ECY3][ECX3] == 'X') ECHIT3 = true;
        std::cout << "Enemy Cruiser has been hit at " << firing << "!" << std::endl;
        ECC++;
        if(ECC == 3) qInfo() << "Enemy Cruiser has sunk!";
    } else {
        count++;
    }
    if((mapEnemy[EBY][EBX] == 'X' && !EBHIT) || (mapEnemy[EBY2][EBX2] == 'X' && !EBHIT2) || (mapEnemy[EBY3][EBX3] == 'X' && !EBHIT3) || (mapEnemy[EBY4][EBX4] == 'X' && !EBHIT4))
    {
        if(mapEnemy[EBY][EBX] == 'X') EBHIT = true;
        if(mapEnemy[EBY2][EBX2] == 'X') EBHIT2 = true;
        if(mapEnemy[EBY3][EBX3] == 'X') EBHIT3 = true;
        if(mapEnemy[EBY4][EBX4] == 'X') EBHIT4 = true;
        std::cout << "Enemy Battleship has been hit at " << firing << "!" << std::endl;
        EBC++;
        if(EBC == 4) qInfo() << "Enemy Battleship has sunk!";
    } else {
        count++;
    }

    if(count == 3)
    {
        std::cout << "Your shot misses at " << playerPastShot << std::endl;
    }
}

// Displays if enemy shot hit, sunk, or missed the ships
void Commands::checkEnemyShot()
{
    int count = 0;
    if((mapPlayer[PDY][PDX] == 'X' && !PDHIT) || (mapPlayer[PDY2][PDX2] == 'X' && !PDHIT2))
    {
        if(mapPlayer[PDY][PDX] == 'X') PDHIT = true;
        if(mapPlayer[PDY2][PDX2] == 'X') PDHIT2 = true;
        std::cout << "Your Destroyer has been hit at " << enemyPastShot << "!" << std::endl;
        PDC++;
        EHD = true;
        if(PDC == 2)
        {
            qInfo() << "Your Destroyer has sunk!";
            EHD = false;
        }
    } else {
        count++;
    }
    if((mapPlayer[PCY][PCX] == 'X' && !PCHIT) || (mapPlayer[PCY2][PCX2] == 'X' && !PCHIT2) || (mapPlayer[PCY3][PCX3] == 'X' && !PCHIT3))
    {
        if(mapPlayer[PCY][PCX] == 'X') PCHIT = true;
        if(mapPlayer[PCY2][PCX2] == 'X') PCHIT2 = true;
        if(mapPlayer[PCY3][PCX3] == 'X') PCHIT3 = true;
        std::cout << "Your Cruiser has been hit at " << enemyPastShot << "!" << std::endl;
        PCC++;
        EHC = true;
        if(PCC == 3)
        {
            qInfo() << "Your Cruiser has sunk!";
            EHC = false;
        }
    } else {
        count++;
    }
    if((mapPlayer[PBY][PBX] == 'X' && !PBHIT) || (mapPlayer[PBY2][PBX2] == 'X' && !PBHIT2) || (mapPlayer[PBY3][PBX3] == 'X' && !PBHIT3) || (mapPlayer[PBY4][PBX4] == 'X' && !PBHIT4))
    {
        if(mapPlayer[PBY][PBX] == 'X') PBHIT = true;
        if(mapPlayer[PBY2][PBX2] == 'X') PBHIT2 = true;
        if(mapPlayer[PBY3][PBX3] == 'X') PBHIT3 = true;
        if(mapPlayer[PBY4][PBX4] == 'X') PBHIT4 = true;
        std::cout << "Your Battleship has been hit at " << enemyPastShot << "!" << std::endl;
        PBC++;
        EHB = true;
        if(PBC == 4)
        {
            qInfo() << "Your Battleship has sunk!";
            EHB = false;
        }
    } else {
        count++;
    }

    if(count == 3) std::cout << "Enemy shot misses at " << enemyPastShot << std::endl;
}

// Checks each round if a player has won
void Commands::checkWin()
{
    if(PDC == 2 && PCC == 3 && PBC == 4)
    {
        gameOver = true;
        showMissed();
        clearConsole();
        displayMap();
        qInfo() << "You Lose! Nice Try :|";
        askToPlayAgain();
    } else if(EDC == 2 && ECC == 3 && EBC == 4)
    {
        gameOver = true;
        showMissed();
        clearConsole();
        displayMap();
        qInfo() << "You Won! Congratulations :)";
        askToPlayAgain();
    } else if(PDC == 2 && PCC == 3 && PBC == 4 && EDC == 2 && ECC == 3 && EBC == 4)
    {
        gameOver = true;
        showMissed();
        clearConsole();
        displayMap();
        qInfo() << "You tied! Good job ;)";
        askToPlayAgain();
    }
}

// Displays missed ships at the end of the game if you lose/tie
void Commands::showMissed()
{
   if(EDHIT != true) shownEnemy[EDY][EDX] = 'D';
   if(EDHIT2 != true) shownEnemy[EDY2][EDX2] = 'D';

   if(ECHIT != true) shownEnemy[ECY][ECX] = 'C';
   if(ECHIT2 != true) shownEnemy[ECY2][ECX2] = 'C';
   if(ECHIT3 != true) shownEnemy[ECY3][ECX3] = 'C';

   if(EBHIT != true) shownEnemy[EBY][EBX] = 'B';
   if(EBHIT2 != true) shownEnemy[EBY2][EBX2] = 'B';
   if(EBHIT3 != true) shownEnemy[EBY3][EBX3] = 'B';
   if(EBHIT4 != true) shownEnemy[EBY4][EBX4] = 'B';
}

void Commands::askToPlayAgain()
{
    reset();
    qInfo() << "Do you want to play again? Enter PLAY";
    std::string answer;
    std::cin >> answer;
    checkReady(answer);
}

// Resets everything to default to start another game
void Commands::reset()
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            mapEnemy[i][j] = '*';
            mapPlayer[i][j] = '*';
            shownEnemy[i][j] = '*';
        }
    }
    gameOver = false;
    EDHIT = false, EDHIT2 = false, ECHIT = false, ECHIT2 = false, ECHIT3 = false, EBHIT = false, EBHIT2 = false, EBHIT3 = false, EBHIT4 = false;
    PDHIT = false, PDHIT2 = false, PCHIT = false, PCHIT2 = false, PCHIT3 = false, PBHIT = false, PBHIT2 = false, PBHIT3 = false, PBHIT4 = false;
    EDC = 0, ECC = 0, EBC = 0, PDC = 0, PCC = 0, PBC = 0;
    eDestroyer = true, eCruiser = true, eBattleship = true;
    pDestroyer = true, pCruiser = true, pBattleship = true;
    EHD = false, EHC = false, EHB = false;
    CC = 3, BC = 3;
}
