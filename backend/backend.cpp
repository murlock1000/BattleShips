#include <iostream>
#include <fstream>

using namespace std;

int main(){
	//these variables might become changeable from frontend later
	
	int tableWidth = 10;
	int tableHeight = 10;
	int shipNumber = 5;
	int playerNumber = 2;

	//this ship length table might also become changeable from frontend later

	int shipHealth [playerNumber] [shipNumber];
	
	shipHealth [0] [0] = 5; //carrier
	shipHealth [1] [0] = 5; //carrier
	shipHealth [0] [1] = 4; //battleship
	shipHealth [1] [1] = 4; //battleship
	shipHealth [0] [2] = 3; //cruiser
	shipHealth [1] [2] = 3; //cruiser
	shipHealth [0] [3] = 3; //submarine
	shipHealth [1] [3] = 3; //submarine
	shipHealth [0] [4] = 2; //destroyer
	shipHealth [1] [4] = 2; //destroyer

	//initialisation
	
	int shipTable [playerNumber] [tableWidth * tableHeight];
	int shipsLeft [playerNumber] = {5,5};

	for (int i = 0; i < playerNumber; i++) {
		for (int j = 0; j < tableWidth * tableHeight; j++) {

			cin >> shipTable [i] [j]; //replace

		}
	}

	int gameEnd = 0;
	int currentPlayer = 0; //player 1 starts
	int opponentPlayer = 1; //playing against the next player

	//main game
	
	while (!gameEnd) {
		int tileX;
		int tileY;

		cin >> tileX >> tileY; //replace

        tileX--;
        tileY--;

		if (shipTable [opponentPlayer] [tableWidth * tileY + tileX] == 0) {

			cout << "0\n"; //replace

		}

		else {
            
			if (--shipHealth [opponentPlayer] [shipTable [opponentPlayer] [tableWidth * tileY + tileX] - 1] == 0) {
				shipsLeft [opponentPlayer]--;

				cout << "2 " << shipTable [opponentPlayer] [tableWidth * tileY + tileX] << "\n"; //replace
			       	
			}

			else {

				cout << "1\n"; //replace

			}

		}

		if (shipsLeft [opponentPlayer] == 0) {
			gameEnd = 1;
			break;
		}

		currentPlayer = (currentPlayer + 1) % playerNumber;
	       	opponentPlayer = (currentPlayer + 1) % playerNumber;	

	}

	return 0;
}
