#include <iostream> //do i really need to tell what this header provides?
#include <bits/stdc++.h> //provides string to char* conversion utility.
#include "stdcomm.h" //Provides AI communication functions prototypes
#include <windows.h>

using namespace std;

//The followiing functions are used to read data.

int dbRead () { //Reads from database. To be implemented.
    //int buffer;
    //cin >> buffer;
    //return buffer;
}

int gameRead (bool playerType, HANDLE child_OUT_Rd) { //Universal read command.
    if (playerType) { //ai
        return stdReadWin(child_OUT_Rd);
    }
    else { //frontend
        return dbRead ();
    }
}

//The following functions are used to write data.

void dbWrite (int buffer) { //Writes to database. To be implemented.
    //cout << buffer << "\n";
}

void gameWrite (int buffer, bool playerType, HANDLE child_IN_Wr) { //Universal write command.
    if (playerType) { //ai
        stdWriteWin (child_IN_Wr, buffer);
    }
    else { //frontend
        dbWrite (buffer);
    }
}

int disconnect (int playerNumber, HANDLE fdOutput[], HANDLE fdInput[], int pid[], bool playerType[]) { //Terminates connections
    int success = 0;

    for (int i = 0; i < playerNumber; i++) {
        if (playerType [i]) { //ai
            stdWriteWin (fdOutput [i], 4); //Tells AI to exit.
            if (stdDisconnectWin (pid [i]) < 0) { //Kills AI in case it hasn't exited yet
                success = -1;
            }
        }
        else { //frontend
        //to be implemented.
        }
    }

    return success;
}

int main (int argc, char* argv []) {
    //Should be read from database later

    int tableWidth = 10;
    int tableHeight = 10;
    int shipNumber = 5;
    int playerNumber = 2;

    //initialisation

    bool playerType [playerNumber];
    int shipHealth [playerNumber] [shipNumber];
    int shipTable [playerNumber] [tableWidth * tableHeight];
    int shipsLeft [playerNumber];
    HANDLE fdInput [playerNumber]; //holds input file descriptors.
    HANDLE fdOutput [playerNumber]; //holds output file descriptors.
    int pid [playerNumber]; //holds ai pids

    for (int i = 0; i < playerNumber; i++) {
        shipsLeft [i] = shipNumber;
        //playerType [i] = dbRead (); //player types are saved in the database.
        playerType [i] = 1; //Temporary hard-coding ai because database isn't ready yet

        if (playerType [i]) { //ai initialisation

            string aiName;
            aiName = "ai_random"; //should be read from db later.

            string aiProcName = aiName + ".exe";
            string aiPath = "./ai/" + aiProcName;

            HANDLE child_IN_Wr = NULL;
            HANDLE child_OUT_Rd = NULL;

            HANDLE aiIO [2]={child_IN_Wr,child_OUT_Rd};

            int* aiPid = new int;

            int stdConnSuccess = stdConnectWin (aiIO, *aiPid, aiPath.c_str(), ("0").c_str());

            if (stdConnSuccess < 0) {
                //If launching an ai process fails, we have to kill parent.
                //However, before that we must terminate other AIs which may have been lauched before.
                //We only need to do that to previous players, so we enter i instead of playerNumber.

                disconnect(i, fdOutput, fdInput, pid, playerType);
                cout << "1 "; //tells server game initialisation failed
                return 1;
            }
            else if (stdConnSuccess > 0) {
                return 0;
            }

            pid [i] = *aiPid;
            fdInput [i] = aiIO [0]; //Store each player's fds
            fdOutput [i] = aiIO [1];

        }
        else { //database initialisation
            //to be implemented
        }

        for (int j=0; j < shipNumber; j++) {
            shipHealth [i] [j] = 0;
        }

        for (int j = 0; j < tableWidth * tableHeight; j++) {
            shipTable [i] [j] = gameRead (playerType [i], fdInput [i]);
            if (shipTable [i] [j] != 0) {
                shipHealth [i] [shipTable [i] [j] - 1] ++;
            }

        }
    }

    int currentPlayer = 0; //player 1 starts
    int opponentPlayer = 1; //playing against the next player

    cout << "0 "; //Telling server that we have successfully initialised a game

    //main game

    while (true) {

        //All cout commands are temporary and for testing purposes only

        cout << currentPlayer << " ";

        int tileX;
        int tileY;

        tileX = gameRead (playerType [currentPlayer], fdInput [currentPlayer]);
        tileY = gameRead (playerType [currentPlayer], fdInput [currentPlayer]);

        cout << tileX << " " << tileY << " ";

        tileX--;
        tileY--;

        if (shipTable [opponentPlayer] [tableWidth * tileY + tileX] == 0) { //Tile isn't taken by any ship

            gameWrite (1, playerType [currentPlayer], fdOutput [currentPlayer]);

            cout << "1 ";

        }

        else {

            if (--shipHealth [opponentPlayer] [shipTable [opponentPlayer] [tableWidth * tileY + tileX] - 1] == 0) { //Target tile is the last tile of some ship

                shipsLeft [opponentPlayer]--;

                if (shipsLeft [opponentPlayer] == 0) { //Game over

                    cout << "4 ";

                    //Write last move and winner to the database

                    break;
                }

                gameWrite (3, playerType [currentPlayer], fdOutput [currentPlayer]);

                cout << "3 ";

            }

            else { //Tile is taken by a ship, but it isn't its last tile

                gameWrite (2, playerType [currentPlayer], fdOutput [currentPlayer]); //write function

                cout << "2 ";

            }

        }

        currentPlayer = (currentPlayer + 1) % playerNumber; //Next player's turn
        opponentPlayer = (currentPlayer + 1) % playerNumber;

    }

    //disconnecting players

    if (disconnect(playerNumber, fdOutput, fdInput, pid, playerType) < 0) {
        return 1;
    }

    return 0;
}
