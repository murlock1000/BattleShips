#include <iostream>
#include <fstream>

using namespace std;

//read functions

int dbRead () {
    //database communication
    int buffer;
    cin >> buffer;
    return buffer;
}

int stdRead () {
    //ai communication
    int buffer;
    cin >> buffer;
    return buffer;
}

int read (bool playerType) {
    if (playerType) { //ai
        return stdRead ();
    }
    else { //frontend
        return dbRead ();
    }
}

//write functions

void dbWrite (int buffer) {
    cout << buffer << "\n";
}

void stdWrite (int buffer) {
    cout << buffer << "\n";
}

void write (int buffer, bool playerType) {
    if (playerType) { //ai
        stdWrite (buffer);
    }
    else { //frontend
        dbWrite (buffer);
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
       
    for (int i = 0; i < playerNumber; i++) {
        shipsLeft [i] = shipNumber;
        playerType [i] = dbRead ();

        for (int j=0; j < shipNumber; j++) {
            shipHealth [i] [j] = 0;
        }

        for (int j = 0; j < tableWidth * tableHeight; j++) {
            shipTable [i] [j] = read (playerType [i]);

            if (shipTable [i] [j] != 0) {
                shipHealth [i] [shipTable [i] [j] - 1] ++;
            } 

        }
    }

    int currentPlayer = 0; //player 1 starts
    int opponentPlayer = 1; //playing against the next player

    //main game

    while (true) {
        int tileX;
        int tileY;

        tileX = read (playerType [currentPlayer]); //read function
        tileY = read (playerType [currentPlayer]); //read function

        tileX--;
        tileY--;

        if (shipTable [opponentPlayer] [tableWidth * tileY + tileX] == 0) {

            write (1, playerType [currentPlayer]); //write function

        }

        else {
            
            if (--shipHealth [opponentPlayer] [shipTable [opponentPlayer] [tableWidth * tileY + tileX] - 1] == 0) {
                shipsLeft [opponentPlayer]--;

                write (3, playerType [currentPlayer]); //write function

            }

            else {

                write (2, playerType [currentPlayer]); //write function

            }

        }

        if (shipsLeft [opponentPlayer] == 0) {
            break;
        }

        currentPlayer = (currentPlayer + 1) % playerNumber;
        opponentPlayer = (currentPlayer + 1) % playerNumber;    

    }
    
    return 0;
}
