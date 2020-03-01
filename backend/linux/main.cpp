#include <iostream>
#include "main.h"

using namespace std;

int main (int argc, char* argv []) {
	string option = "";
	if (argc > 1) {
		option = argv [1];
	}	
	if (option == "-s" || option == "--server") { //launching server
		startServer ();
	}
	else if ((option == "-g" || option == "--game") && argc == 3) { //launching game
		int lobbyId = stoi (argv [2]);
		startGame (lobbyId);
	}
	else if ((option == "-r" || option == "--register") && argc > 2) { //launching register-ai
		for (int i = 2; i < argc; i ++) {
			registerAI (argv [i]);
		}
	}
	else { //either something was typed in incorrectly or '--help' was issued
		cout << "USAGE: battleships-server OPTION [PARAMETERS]\n";
		cout << "Options:\n";
		cout << "-s, --server                                           Launch a server\n";
		cout << "-g, --game LOBBY_ID                                    Launch a single game (for debugging purposes only)\n";
		cout << "-r, --register AI_NAME [AI_2_NAME] ...                 Register provided AIs\n";
		cout << "-h, --help                                             Display this help screen and quit\n";
		cout << "Only one option can be selected at the time\n";
	}
}
