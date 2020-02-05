#include <iostream>
#include "stdcomm.h"
#include <string>
#include <vector>
#include "dbconnector.h"

//Windows compatibility

#ifdef __linux__
#define HANDLE int
#endif

using namespace std;

int main() {

    DBconnector dbc;
    dbc.Connect("127.0.0.1", "root", "root", "battleships"); //connecting to database
 string gamePath = "./game.exe";
 string gameProc = "game.exe ";
 

 vector<pair<int, int>> lobbyTimeout;

 
    while (true) {

        vector<int> lobbies = dbc.GetReadyLobbies(); //getting that lobby info
        for (int i = 0; i < lobbies.size(); i++) {

            //cout << "Lobby " << lobbies[i] << " ready\n";

            HANDLE unusedIO[2];
            int* unusedPid = new int;

            stringstream lobbyId_s;
            lobbyId_s << lobbies[i];


            string lobbyId_string = gameProc+" "+lobbyId_s.str();


            int stdConnSuccess = stdConnect(unusedIO, unusedPid, gamePath.c_str(),lobbyId_string);

            if (stdConnSuccess < 0) {
                cout << "ERROR: Failed to launch lobby " << lobbies[i] << "\n";
            }
            else if (stdConnSuccess > 0) {
                //finally ends finished game process
                return 0;
            }

            int intOut = stdRead(unusedIO[0]);
            if (intOut == -1) {
                cout << "FAILED TO LAUNCH LOBBY WITH ID: " << lobbyId_string << endl;
                return -1;
            }
            else {
                cout << "Launched lobby with ID: " << lobbyId_string << endl;
            }

        }
    }
 return 0;
}
