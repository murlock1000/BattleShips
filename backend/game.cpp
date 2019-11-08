#include <iostream> //do i really need to tell what this header provides?
#include <bits/stdc++.h> //provides string to char* conversion utility.
#include "stdcomm.h" //Provides AI communication functions prototypes

using namespace std;

//The followiing functions are used to read data.

int dbRead () { //Reads from database. To be implemented.
    int buffer;
    cin >> buffer;
    return buffer;
}

int gameRead (bool playerType, int fileDesc) { //Universal read command.
    if (playerType) { //ai
        return stdRead (fileDesc);
    }
    else { //frontend
        return dbRead ();
    }
}

//The following functions are used to write data.

void dbWrite (int buffer) { //Writes to database. To be implemented.
    cout << buffer << "\n";
}

void gameWrite (int buffer, bool playerType, int fileDesc) { //Universal write command.
    if (playerType) { //ai
        stdWrite (fileDesc, buffer);
    }
    else { //frontend
        dbWrite (buffer);
    }
}

void disconnect (int playerNumber, int fdOutput[], int fdInput[], bool playerType[]) { //Terminates connections
     for (int i = 0; i < playerNumber; i++) {
        if (playerType [i]) { //ai
            stdWrite (fdOutput [i], 4); //Tells AI to exit.
        }
        else { //frontend
        //to be implemented.
        }
    }
}

int main () {
    //these variables might become changeable from frontend later
    
    int tableWidth = 10;
    int tableHeight = 10;
    int shipNumber = 5;
    int playerNumber = 2;

    //initialisation
    
    bool playerType [playerNumber];
    int shipHealth [playerNumber] [shipNumber];
    int shipTable [playerNumber] [tableWidth * tableHeight];
    int shipsLeft [playerNumber];
    int fdInput [playerNumber]; //holds input file descriptors.
    int fdOutput [playerNumber]; //holds output file descriptors.
    int pid [playerNumber]; //holds ai pids
       
    for (int i = 0; i < playerNumber; i++) {
        shipsLeft [i] = shipNumber;
        playerType [i] = dbRead (); //player types are saved in the database.

        if (playerType [i]) { //ai initialisation

            string aiName;
            cin >> aiName; //should be read from db later.
            
            string aiProcName = aiName + ".exe";
            string aiPath = "../ai/" + aiProcName;
            
            int aiIO [2];

            int* aiPid = new int;

            int stdConnSuccess = stdConnect (aiIO, aiPid, aiPath.c_str(), aiProcName.c_str());

            if (stdConnSuccess < 0) {
                //If launching an ai process fails, we have to kill parent.
                //However, before that we must terminate other AIs which may have been lauched before.
                //We only need to do that to previous players, so we enter i instead of playerNumber.

                disconnect(i, fdOutput, fdInput, playerType);
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

    //main game

    while (true) {

        //Of course, it's safe to output some game progress.

        cout << "Player " << currentPlayer + 1 << " move:\n";

        int tileX;
        int tileY;

        tileX = gameRead (playerType [currentPlayer], fdInput [currentPlayer]);
        tileY = gameRead (playerType [currentPlayer], fdInput [currentPlayer]);

        cout << tileX << " " << tileY << "\n\n";

        tileX--;
        tileY--;

        if (shipTable [opponentPlayer] [tableWidth * tileY + tileX] == 0) { //Tile isn't taken by any ship

            gameWrite (0, playerType [currentPlayer], fdOutput [currentPlayer]);
        
            cout << "MISSED!\n\n"; 

        }

        else {
            
            if (--shipHealth [opponentPlayer] [shipTable [opponentPlayer] [tableWidth * tileY + tileX] - 1] == 0) { //Target tile is the last tile of some ship
        
                cout << "SHIP SUNK!\n\n";

                shipsLeft [opponentPlayer]--;

                if (shipsLeft [opponentPlayer] == 0) { //Game over

                    cout << "PLAYER " << currentPlayer + 1 << " WON!\n\n";

                    //Write last move and winner to the database

                    break;
                }

                gameWrite (3, playerType [currentPlayer], fdOutput [currentPlayer]);

            }

            else { //Tile is taken by a ship, but it isn't its last tile

                gameWrite (2, playerType [currentPlayer], fdOutput [currentPlayer]); //write function
        
                cout << "SHIP HIT!\n\n";

            }

        }

        currentPlayer = (currentPlayer + 1) % playerNumber; //Next player's turn
        opponentPlayer = (currentPlayer + 1) % playerNumber;    

    }

    //disconnecting players
    
    disconnect(playerNumber, fdOutput, fdInput, playerType);
    return 0;
}
