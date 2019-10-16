#include <iostream>
#include <bits/stdc++.h>

using namespace std;

//read functions

int dbRead () {
    //database communication
    int buffer;
    cin >> buffer;
    return buffer;
}

int stdConnect (int childIO [2], const char* childPath, const char* childProcName);

int stdRead (int fileDesc);

int gameRead (bool playerType, int fileDesc) {
    if (playerType) { //ai
        return stdRead (fileDesc);
    }
    else { //frontend
        return dbRead ();
    }
}

//write functions

void dbWrite (int buffer) {
    cout << buffer << "\n";
}

void stdWrite (int fileDesc, int buffer);

void gameWrite (int buffer, bool playerType, int fileDesc) {
    if (playerType) { //ai
        stdWrite (fileDesc, buffer);
    }
    else { //frontend
        dbWrite (buffer);
    }
}

void disconnect (int playerNumber, int fdOutput[], int fdInput[], bool playerType[]) {
     for (int i = 0; i < playerNumber; i++) {
        if (playerType [i]) { //ai
            stdWrite (fdOutput [i], 4);
            //close (fdInput [i]);
            //close (fdOutput [i]);
        }
        else { //frontend

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
    int fdInput [playerNumber];
    int fdOutput [playerNumber];
       
    for (int i = 0; i < playerNumber; i++) {
        shipsLeft [i] = shipNumber;
        playerType [i] = dbRead ();

        if (playerType [i]) { //ai initialisation

            string aiName;
            cin >> aiName;
            
            string aiProcName = aiName + ".exe";
            string aiPath = "../ai/" + aiProcName;
            
            int aiIO [2];

            int stdConnSuccess = stdConnect (aiIO, aiPath.c_str(), aiProcName.c_str());

            if (stdConnSuccess < 0) {
                disconnect(i, fdOutput, fdInput, playerType);
                return 1;
            }
            else if (stdConnSuccess > 0) {
                return 0;
            }

            fdInput [i] = aiIO [0];
            fdOutput [i] = aiIO [1];

        }
        else { //database initialisation
            
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

        cout << "Player " << currentPlayer + 1 << " move:\n";

        int tileX;
        int tileY;

        tileX = gameRead (playerType [currentPlayer], fdInput [currentPlayer]); //read function
        tileY = gameRead (playerType [currentPlayer], fdInput [currentPlayer]); //read function

        cout << tileX << " " << tileY << "\n\n";

        tileX--;
        tileY--;

        if (shipTable [opponentPlayer] [tableWidth * tileY + tileX] == 0) {

            gameWrite (0, playerType [currentPlayer], fdOutput [currentPlayer]); //write function
        
            cout << "MISSED!\n\n"; 

        }

        else {
            
            if (--shipHealth [opponentPlayer] [shipTable [opponentPlayer] [tableWidth * tileY + tileX] - 1] == 0) {
        
                shipsLeft [opponentPlayer]--;

                gameWrite (3, playerType [currentPlayer], fdOutput [currentPlayer]); //write function
        
                cout << "SHIP SUNK!\n\n";

            }

            else {

                gameWrite (2, playerType [currentPlayer], fdOutput [currentPlayer]); //write function
        
                cout << "SHIP HIT!\n\n";

            }

        }

        if (shipsLeft [opponentPlayer] == 0) {
            break;
        }

        currentPlayer = (currentPlayer + 1) % playerNumber;
        opponentPlayer = (currentPlayer + 1) % playerNumber;    

    }

    //disconnecting players
    
    disconnect(playerNumber, fdOutput, fdInput, playerType);
    return 0;
}
