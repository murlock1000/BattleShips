#include <iostream>
#include "stdcomm.h"
#include <string>
#include <bits/stdc++.h>
#include <vector>
#include "dbconnector/dbconnector.h"

using namespace std;

int main() {

    DBconnector dbc;
    dbc.Connect ("127.0.0.1", "root", "password", "battleships"); //connecting to database
    
    while (true) {

        vector<int> lobbies = dbc.GetReadyLobbies(); //getting that lobby info
        for (int i = 0; i < lobbies.size(); i++) {

            //cout << "Lobby " << lobbies[i] << " ready\n";

            int unusedIO [2];
            int* unusedPid = new int;

            int stdConnSuccess = stdConnect (unusedIO, unusedPid, "./game.exe", "game.exe", to_string(lobbies[i]).c_str());

            if (stdConnSuccess < 0) {
                 cout << "ERROR: Failed to launch lobby " << lobbies[i] << "\n";
            }
            else if (stdConnSuccess > 0) {
                //finally ends finished game process
                return 0;
            }
        }
    }
    return 0;
}
