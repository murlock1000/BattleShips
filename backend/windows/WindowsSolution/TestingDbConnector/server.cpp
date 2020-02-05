#include "pch.h"
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

//int main() {
//
//   // cout << "a" << endl;
//
//    DBconnector dbc;
//    dbc.Connect("127.0.0.1", "root", "root", "battleships"); //connecting to database
//
//    while (true) {
//
//        vector<int> lobbies = dbc.GetReadyLobbies(); //getting that lobby info
//        for (int i = 0; i < lobbies.size(); i++) {
//
//            //cout << "Lobby " << lobbies[i] << " ready\n";
//
//            HANDLE unusedIO[2];
//            int* unusedPid = new int;
//
//            stringstream lobbyId_s;
//            lobbyId_s << lobbies[i];
//            const char* lobbyId_c = lobbyId_s.str().c_str();
//
//            int stdConnSuccess = stdConnect(unusedIO, unusedPid, "./game.exe", "game.exe", lobbyId_c);
//
//            if (stdConnSuccess < 0) {
//                cout << "ERROR: Failed to launch lobby " << lobbies[i] << "\n";
//            }
//            else if (stdConnSuccess > 0) {
//                //finally ends finished game process
//                return 0;
//            }
//        }
//    }
//    return 0;
//}
