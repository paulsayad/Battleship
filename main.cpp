#include <QCoreApplication>
#include <QDebug>
#include <iostream>
#include <QTimer>
#include <QRandomGenerator>
#include "commands.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::string answer;
    Commands game;

    qInfo() << "Welcome to Paul Sayad's Battleship Game Engine";
    qInfo() << "Are you ready to start a game? Enter PLAY or HOWTOPLAY";
    std::cin >> answer;
    game.checkReady(answer);

    return a.exec();
}

// WHAT TO DO TODAY:
/*
 * Suggestions:*
 *
 * BUGS:
 * W hen inputting letter space number the error message came twice
 */

/* later on stuff
 * - create QUIT command
 * - add help to explain how to play
 */
