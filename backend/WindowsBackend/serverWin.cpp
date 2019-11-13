#include <iostream>
#include "stdcomm.h"
#include <string>
#include <bits/stdc++.h>
#include <windows.h>

using namespace std;

int main() {
    while (true) {
        //This is where server would be looking for pending lobbies
        //However, since database isn't implemented yet, we will instead launch a new hardcoded game

        int lobbyId = 0;
        stringstream ss;
        ss<<lobbyId;
        string lobbyId_s = ss.str(); //to_string(lobbyId); to_string nera windows g++ versijoje
        const char* lobbyId_c = lobbyId_s.c_str();

        HANDLE child_IN_Wr = NULL;
        HANDLE child_OUT_Rd = NULL;

        HANDLE unusedIO [2]={child_IN_Wr,child_OUT_Rd}; //Will not be used in a future, although it's currently used for testing purposes

        int* unusedPid = new int;

        if (stdConnectWin (unusedIO, unusedPid, "./game.exe", lobbyId_c) < 0) {
            //Normally, server would send an error code to database and continue working
            //However, due to our testing we can't let server continue
            cout << "ERROR: Failed to launch game\n";
            return 1;
        }

        //Usually we would leave game.exe alone here and go back to checking database for new games
        //If an error occurs game.exe would inform database itself
        //However, we do not have working database yet, so we need to make sure the game is working properly
        //Therefore, this server can currently play only one game, because we will be reading gameplay information

        while (true) {
            int a, b, c, d;
            a = stdReadWin (unusedIO [0]);
            b = stdReadWin (unusedIO [0]);
            c = stdReadWin (unusedIO [0]);
            d = stdReadWin (unusedIO [0]);

            cout << "PLAYER " << a+1 << " MOVE:\n" << b << " " << c << "\n\n";

            if (d == 1) cout << "MISSED!\n\n";
            else if (d == 2) cout << "SHIP HIT!\n\n";
            else cout << "SHIP SUNK!\n\n";
            if (d == 4) {
                cout << "PLAYER " << a+1 << " WON!\n\n";
                break;
            }
        }
        //The game has finished. Due to our testing we will now stop the server
        return 0;
    }
    return 0;
}
