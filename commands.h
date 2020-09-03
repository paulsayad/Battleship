#ifndef COMMANDS_H
#define COMMANDS_H
#define underlineON "\033[4m"
#define underlineOFF "\033[0m"

#include <QObject>
#include <iostream>
#include <QDebug>
#include <QProcess>
#include <QCoreApplication>
#include <QRandomGenerator>

class Commands : public QObject
{
    // The enemy map with the enemy ships
    char mapEnemy[8][8] = {{'*','*','*','*','*','*','*','*'},
                           {'*','*','*','*','*','*','*','*'},
                           {'*','*','*','*','*','*','*','*'},
                           {'*','*','*','*','*','*','*','*'},
                           {'*','*','*','*','*','*','*','*'},
                           {'*','*','*','*','*','*','*','*'},
                           {'*','*','*','*','*','*','*','*'},
                           {'*','*','*','*','*','*','*','*'}};

    // The players map with the player ship
    char mapPlayer[8][8] = {{'*','*','*','*','*','*','*','*'},
                            {'*','*','*','*','*','*','*','*'},
                            {'*','*','*','*','*','*','*','*'},
                            {'*','*','*','*','*','*','*','*'},
                            {'*','*','*','*','*','*','*','*'},
                            {'*','*','*','*','*','*','*','*'},
                            {'*','*','*','*','*','*','*','*'},
                            {'*','*','*','*','*','*','*','*'}};

    //The enemy  map that will be displayed with the ships hidden
    char shownEnemy[8][8] = {{'*','*','*','*','*','*','*','*'},
                            {'*','*','*','*','*','*','*','*'},
                            {'*','*','*','*','*','*','*','*'},
                            {'*','*','*','*','*','*','*','*'},
                            {'*','*','*','*','*','*','*','*'},
                            {'*','*','*','*','*','*','*','*'},
                            {'*','*','*','*','*','*','*','*'},
                            {'*','*','*','*','*','*','*','*'}};

    bool gameOver = false;    // Switch to end the game
    std::string firing;       // String to store player's shot
    int EDX, EDY, EDX2, EDY2, PDX, PDY, PDX2, PDY2;                                                     // Coordinates of each part of both destroyer ships
    int ECX, ECY, ECX2, ECY2, ECX3, ECY3, PCX, PCY, PCX2, PCY2, PCX3, PCY3;                             // Coordinates of each part of both cruiser ships
    int EBX, EBY, EBX2, EBY2, EBX3, EBY3, EBX4, EBY4, PBX, PBY, PBX2, PBY2, PBX3, PBY3, PBX4, PBY4;     // Coordinates of each part of both battleship ships
    bool EDHIT = false, EDHIT2 = false, ECHIT = false, ECHIT2 = false, ECHIT3 = false, EBHIT = false, EBHIT2 = false, EBHIT3 = false, EBHIT4 = false; // Variables to store enemy ship parts that are hit
    bool PDHIT = false, PDHIT2 = false, PCHIT = false, PCHIT2 = false, PCHIT3 = false, PBHIT = false, PBHIT2 = false, PBHIT3 = false, PBHIT4 = false; // Variables to store player ship parts that are hit
    int EDC = 0, ECC = 0, EBC = 0, PDC = 0, PCC = 0, PBC = 0;       // Counters used to determine if the whole ship is sunk
    bool eDestroyer = true, eCruiser = true, eBattleship = true;    // Switches used to insert each square of the enemy ships
    bool pDestroyer = true, pCruiser = true, pBattleship = true;    // Switches used to insert each square of the player ships
    std::string enemyPastShot, playerPastShot;                      // Stores the past shots of both player and enemy
    bool EHD = false, EHC = false, EHB = false;                     // Variables for each enemy ship, and stores if they are hit or not
    std::string cdir, cog, bdir, bog;                               // Strings used to store direction and original area hit for enemys ship finder
    int CC = 3, BC = 3;                                             // Intergers used in the enemys remaining ship finder

    Q_OBJECT
public:
    explicit Commands(QObject *parent = nullptr);
    void checkReady(std::string answer); // Checks if the player is ready to start the game
    void howToPlay();                    // Displays the how to play screen
    void startGame();                    // Starts the game by randomly creating the map and displaying it
    void insertBoats();                  // Randomly inserts the start for each boat
    void checkExpansion();               // Checks the direction the boats can expand
    void expandBoat(std::string player, std::string boat, std::string direction); // Expands the boats in the set directions
    void displayMap();                   // Displays the maps on terminal for the player
    void clearConsole();                 // Clears the terminal
    void beginShots();                   // Allows the game to start taking shots
    void takeShot();                     // Allows the player to fire
    std::string pickEnemyShot();         // Randomly picks where the enemy will shoot unless enemy got hit last time, then it will find the remaining parts of the ship
    void enemyShot();                    // Makes the enemy fire
    bool verifyShot(std::string player, std::string shot); // Verifies if player or enemy are firing at a valid location
    void checkPlayerShot();              // Displays if player shot hit, sunk, or missed the ships
    void checkEnemyShot();               // Displays if enemy shot hit, sunk, or missed the ships
    void checkWin();                     // Checks each round if a player has won
    void showMissed();                   // Displays missed ships at the end of the game if you lose/tie
    void askToPlayAgain();
    void reset();                        // Resets everything to default to start another game

signals:

};

#endif // COMMANDS_H
