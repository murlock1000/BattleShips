#include <iostream> //do i really need to tell what this header provides?
#include <bits/stdc++.h> //provides string to char* conversion utility.
#include <string>
#include "stdcomm.h" //Provides AI communication functions prototypes
#include "dbconnector/dbconnector.h" //Provides database communication functions prototypes
#include <cstdlib>
#include <ctime>

using namespace std;

int disconnect (int playerNumber, int fdOutput[], int fdInput[], int pid[], bool playerType[]) { //Terminates connections
    int success = 0;

    for (int i = 0; i < playerNumber; i++) {
        if (playerType [i]) { //ai
            stdWrite (fdOutput [i], 4); //Tells AI to exit.
            if (stdDisconnect (pid [i]) < 0) { //Kills AI in case it hasn't exited yet
                success = -1;
            }
        }
        else { //frontend
        //to be implemented.
        }
    }

    return success;
}

int main (int argc, char* argv []) {

    if (argc != 2) { //must have exactly one argument (not counting process name)
        cerr << "game: Must be launched with exactly one argument\n";
        return 1;
    }

    DBconnector dbc;
    dbc.Connect ("127.0.0.1", "root", "password", "battleships"); //connecting to database

    int lobbyId = stoi(argv[1]); //reads lobbyId from a provided argument
    DBconnector::ConsoleReadStruct lobby = dbc.ConsoleRead (lobbyId); //gets information about lobby
    
    //May be saved in lobby table later

    int tableWidth = 10;
    int tableHeight = 10;
    int shipNumber = 5;
    int playerNumber = 2;

    //--------------------INITIALISATION--------------------
    
    bool playerType [playerNumber];
    int playerId [playerNumber]; 
    int shipHealth [playerNumber] [shipNumber];
    int shipTable [playerNumber] [tableWidth * tableHeight];
    int shipsLeft [playerNumber];
    int fdInput [playerNumber]; //holds input file descriptors.
    int fdOutput [playerNumber]; //holds output file descriptors.
    int pid [playerNumber]; //holds ai pids

    //initialising players

    srand (time(NULL));
    int admin = rand() % playerNumber; //randomises which player plays first
       
    for (int i = 0; i < playerNumber; i++) {
        shipsLeft [i] = shipNumber;

        for (int j = 0; j < shipNumber; j++) {
            shipHealth [i] [j] = 0;
        }

        DBconnector::UserInfoTable userInfo; //get info about user

        if (i == admin) {
            userInfo = dbc.GetUserInfo (lobby.adminID);
            playerId [i] = lobby.adminID;
        }

        else {
            userInfo = dbc.GetUserInfo (lobby.opponentID);
            playerId [i] = lobby.opponentID;
        }

        playerType [i] = userInfo.is_ai;

        if (playerType [i]) { //ai initialisation

            string aiName = userInfo.username;
            
            string aiProcName = aiName + ".exe";
            string aiPath = "./ai/" + aiProcName;
            
            int aiIO [2];

            int* aiPid = new int;

            int stdConnSuccess = stdConnect (aiIO, aiPid, aiPath.c_str(), aiProcName.c_str(), "0");

            if (stdConnSuccess < 0) {
                //If launching an ai process fails, we have to kill parent.
                //However, before that we must terminate other AIs which may have been lauched before.
                //We only need to do that to previous players, so we enter i instead of playerNumber.

                disconnect(i, fdOutput, fdInput, pid, playerType);
                cout << "1 "; //tells server game initialisation failed
                return 1;
            }
            else if (stdConnSuccess > 0) { //code executed by child
                return 0;
            }

            pid [i] = *aiPid;
            fdInput [i] = aiIO [0]; //Store each player's fds
            fdOutput [i] = aiIO [1];

            //generating ai ship string which will be sent to history and lobby tables

            string aiShipTable = "";

            for (int j = 0; j < tableWidth * tableHeight; j++) {
                shipTable [i] [j] = stdRead (fdInput [i]); //reading ship table from ai

                aiShipTable += to_string(shipTable [i] [j]);

                if (shipTable [i] [j] != 0) {
                    shipHealth [i] [shipTable [i] [j] - 1] ++;
                } 

            }

            lobby = dbc.ConsoleRead (lobbyId); //updating local information to prevent overwriting ship tables
            
            if (i == admin) {
                dbc.UpdateLobby (lobbyId, "r", lobby.user_input, "", aiShipTable, lobby.opponent_map, 0, "n", 0);
            }
            else {
                dbc.UpdateLobby (lobbyId, "r", lobby.user_input, "", lobby.admin_map, aiShipTable, 0, "n", 0); 
            }
        }

        else { //human player initialisation

            string userShipTable;
            
            if (i == admin) {
                userShipTable = lobby.admin_map;
            }
            else {
                userShipTable = lobby.opponent_map;
            }

            //converting ship string to a ship table

            for (int j = 0; j < tableWidth * tableHeight; j++) {
                shipTable [i] [j] = stoi (userShipTable.substr (j, 1));

                if (shipTable [i] [j] != 0) {
                    shipHealth [i] [shipTable [i] [j] - 1] ++;
                }
            }
        }

    }

    int currentPlayer = 0; //player 1 starts
    int opponentPlayer = 1; //playing against the next player

    lobby = dbc.ConsoleRead (lobbyId); //update local info before launching the game proper
    dbc.UpdateLobby (lobbyId, "i", lobby.user_input, "", lobby.admin_map, lobby.opponent_map, 0, "n", playerId [currentPlayer]); //Set lobby_status to i before telling server initialisation succeeded to prevent it from launching multiple instances of the same lobby

    cout << "0 " << flush; //Telling server that we have successfully initialised a game (flush force sends a line buffer, otherwise data is only sent when the game finishes)

    //--------------------GAME PROPER--------------------
    
    int historyId = dbc.CreateHistoryTable ("Lobby " + to_string(lobbyId), lobby.adminID, lobby.opponentID, lobby.admin_map, lobby.opponent_map);

    int moveId = 0;

    string consoleOutput = "";

    while (true) {

        dbc.UpdateLobby (lobbyId, "i", lobby.user_input, consoleOutput, lobby.admin_map, lobby.opponent_map, historyId, "w", playerId [currentPlayer]); //send backend response to database and tell frontend that game is waiting for its input

        int tileX;
        int tileY;

        if (playerType [currentPlayer] == 0) {
            //frontend
            do { //wait until frontend moves
                lobby = dbc.ConsoleRead (lobbyId); 
            } while (lobby.game_status != "c");

            //read user's move

            int dashPosition = 0;
            while (lobby.user_input.substr(dashPosition, 1) != "-") dashPosition++;
            tileX = stoi(lobby.user_input.substr(0, dashPosition));
            tileY = stoi(lobby.user_input.substr(dashPosition + 1, lobby.user_input.length() - 1));
        }
        else {
            //ai
            tileX = stdRead (fdInput [currentPlayer]);
            tileY = stdRead (fdInput [currentPlayer]);
        }

        tileX--;
        tileY--;

        int response; //tells move consequences (missed, hit or sunk a ship)
        int subresponse; //tells which ship was sunk (only used when a ship was sunk)

        if (shipTable [opponentPlayer] [tableWidth * tileY + tileX] == 0) { //Tile isn't taken by any ship

            response = 1; //missed
        
        }

        else {
            
            if (--shipHealth [opponentPlayer] [shipTable [opponentPlayer] [tableWidth * tileY + tileX] - 1] == 0) { //Target tile is the last tile of some ship

                shipsLeft [opponentPlayer]--;

                if (shipsLeft [opponentPlayer] == 0) { //Game over
                    //write last move to lobby and move tables
                    dbc.UpdateLobby (lobbyId, "i", lobby.user_input, "2-" + to_string (shipTable [opponentPlayer] [tableWidth * tileY + tileX]), lobby.admin_map, lobby.opponent_map, historyId, "f", playerId [currentPlayer]);

                    string pseudoInput, pseudoOutput;
                    pseudoInput = to_string (tileX + 1) + "-" + to_string (tileY + 1);
                    pseudoOutput = "2-" + to_string (shipTable [opponentPlayer] [tableWidth * tileY + tileX]);
                    dbc.UpdateMoveTable (historyId, moveId, pseudoInput, pseudoOutput, playerId [currentPlayer]); 

                    //write winner to database (to be implemented)

                    break;
                }

                //not game over; ship sunk

                response = 3;
                
                subresponse = shipTable [opponentPlayer] [tableWidth * tileY + tileX];
               
            }

            else { //Tile is taken by a ship, but it isn't its last tile

                response = 2; //ship hit
        
            }

        }

        
        string pseudoOutput = to_string (response - 1); //pseudoInput and pseudoOutput are used to send data that has not necessarily been generated to moves table
        if (response == 3) {
            pseudoOutput = pseudoOutput + "-" + to_string (subresponse);
        }

        string pseudoInput = to_string (tileX + 1) + "-" + to_string (tileY + 1);

        dbc.UpdateMoveTable (historyId, moveId, pseudoInput, pseudoOutput, playerId [currentPlayer]);
        
        if (playerType [currentPlayer] == 0) {
            //send console response to lobby table
            consoleOutput = pseudoOutput;
        }
        else {
            //send console response directly to ai
            stdWrite (fdOutput [currentPlayer], response);
        }

        currentPlayer = (currentPlayer + 1) % playerNumber; //Next player's turn
        opponentPlayer = (currentPlayer + 1) % playerNumber;
        
        moveId ++;

    }

    //--------------------DISCONNECTION--------------------

    //disconnecting players
        
    disconnect (playerNumber, fdOutput, fdInput, pid, playerType);

    //deleting lobby
    
    do {
        lobby = dbc.ConsoleRead (lobbyId); //waiting for user to aknowledge end
    } while (lobby.game_status != "c");

    dbc.InitiateDeletion (lobbyId);
        
    return 0;
}
