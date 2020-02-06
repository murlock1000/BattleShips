#include <iostream>
#include "stdcomm.h"
#include <string>
#include <bits/stdc++.h>
#include <vector>
#include "dbconnector/dbconnector.h"
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

void zombiePrevention (int signum) {
    wait (NULL);
}

int main() {

    DBconnector dbc;
    dbc.Connect ("127.0.0.1", "ServerAdmin", "admin", "battleships"); //connecting to database
    
    while (true) {

        vector<int> lobbies = dbc.GetReadyLobbies(); //getting that lobby info
        for (int i = 0; i < lobbies.size(); i++) {

            //cout << "Lobby " << lobbies[i] << " ready\n";

            int unusedIO [2];
            int* unusedPid = new int;

            stringstream lobbyId_s;
            lobbyId_s << lobbies [i];
            const char* lobbyId_c = lobbyId_s.str().c_str();

            int stdConnSuccess = stdConnect (unusedIO, unusedPid, "./game", "game", lobbyId_c);

            if (stdConnSuccess < 0) {
                 cerr << "server: Failed to launch lobby " << lobbies[i] << "\n";
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
