#include <iostream>
#include "stdcomm.h"
#include <string>
#include <vector>
#include "dbconnector.h"
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

void zombiePrevention (int signum) {
	wait (NULL);
}

int startServer () {

	DBconnector dbc;

	if (dbc.Connect ("127.0.0.1", "ServerAdmin", "admin", "battleships") < 0) { //connecting to database
		cerr << "server: (error) Unable to connect to database\n";
		return 1;
	}
	
	cerr << "server: (info) Server is active, looking for pending lobbies\n";

	while (true) {

		vector<int> lobbies = dbc.GetReadyLobbies(); //getting that lobby info
		for (int i = 0; i < lobbies.size(); i++) {

			//cout << "Lobby " << lobbies[i] << " ready\n";

			int unusedIO [2];
			int* unusedPid = new int;

			cerr << "server: (info) Launching game " << lobbies [i] << "\n";

			stringstream lobbyId_s;
			lobbyId_s << lobbies [i];
			const char* lobbyId_c = lobbyId_s.str().c_str();

			int stdConnSuccess = stdConnect (unusedIO, unusedPid, "/usr/local/bin/battleships-server", "battleships-server", "-g", lobbyId_c);

			if (stdConnSuccess < 0) {
				stdConnSuccess = stdConnect (unusedIO, unusedPid, "/usr/bin/battleships-server", "battleships-server", "-g", lobbyId_c);

				if (stdConnSuccess < 0) {
					cerr << "server: (error) Failed to launch lobby " << lobbies[i] << "\n";
				}
				else if (stdConnSuccess > 0) {
					//finally ends finished game process
				return 0;
				}
				else {
					signal (SIGCHLD, zombiePrevention);
				}
				
			}
			else if (stdConnSuccess > 0) {
				//finally ends finished game process
				return 0;
			}
			else {
				signal (SIGCHLD, zombiePrevention);
			}

			stdClosePipes (unusedIO);
		}
	}
	return 0;
}
