#include <iostream>
#include <fstream>

using namespace std;

//read function

int read () {
    int buffer;
    cin >> buffer;
    return buffer;
}

//write function

void write (int buffer) {
    cout << buffer << "\n";
}

int main () {
    //these variables might become changeable from frontend later
    
    int tableWidth = 10;
    int tableHeight = 10;
    int shipNumber = 5;
    int playerNumber = 2;

    //initialisation
    
    int shipHealth [playerNumber] [shipNumber];
    int shipTable [playerNumber] [tableWidth * tableHeight];
    int shipsLeft [playerNumber];

    for (int i = 0; i < playerNumber; i++) {
        shipsLeft [i] = shipNumber;

        for (int j=0; j < shipNumber; j++) {
            shipHealth [i] [j] = 0;
        }

        for (int j = 0; j < tableWidth * tableHeight; j++) {
            shipTable [i] [j] = read (); //read function
            
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

        tileX = read (); //read function
        tileY = read (); //read function

        tileX--;
        tileY--;

        if (shipTable [opponentPlayer] [tableWidth * tileY + tileX] == 0) {

            write (-1); //write function

        }

        else {
            
            if (--shipHealth [opponentPlayer] [shipTable [opponentPlayer] [tableWidth * tileY + tileX] - 1] == 0) {
                shipsLeft [opponentPlayer]--;

                /*write (2); //write function
                write (shipTable [opponentPlayer] [tableWidth * tileY + tileX]); //write function*/
                    
            }

            else {

                //write (1); //write function

            }

            write (1);

        }

        if (shipsLeft [opponentPlayer] == 0) {
            break;
        }

        currentPlayer = (currentPlayer + 1) % playerNumber;
        opponentPlayer = (currentPlayer + 1) % playerNumber;    

    }
    
    return 0;
}
