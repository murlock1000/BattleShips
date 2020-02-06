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

    stdClosePipes (fdInput);
    stdClosePipes (fdOutput);

    return success;
}

int main (int argc, char* argv []) {
stringstream ss;
    if (argc != 2) { //must have exactly one argument (not counting process name)
        cerr << "game: (error) Must be launched with exactly one argument\n";
        return 1;
    }

    DBconnector dbc;

    dbc.Connect ("127.0.0.1", "ServerAdmin", "admin", "battleships"); //connecting to database

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

            string aiPath = "./ai/" + aiName;

            int aiIO [2];

            int* aiPid = new int;

            int stdConnSuccess = stdConnect (aiIO, aiPid, aiPath.c_str(), aiName.c_str(), "0");

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

                ss<<shipTable [i] [j];

                aiShipTable += ss.str();
                ss.str("");
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

            dbc.UpdateLobby (lobbyId, "r", lobby.user_input, "", lobby.admin_map, lobby.opponent_map, 0, "w", playerId [i]);

            cerr << "game: (info) Waiting for frontend to send player " << playerId [i] << " map for lobby " << lobbyId << "\n";

            do {
                lobby = dbc.ConsoleRead (lobbyId);
            } while (lobby.game_status != "c");

            cerr << "game: (info) Receiving player " << playerId [i] << " map for lobby " << lobbyId << "\n";

            string userShipTable;

            if (i == admin) {
                userShipTable = lobby.admin_map;
            }
            else {
                userShipTable = lobby.opponent_map;
            }

            //converting ship string to a ship table

            for (int j = 0; j < tableWidth * tableHeight; j++) {
                
                do { 
                    lobby = dbc.ConsoleRead (lobbyId);
                } while (lobby.game_status != "c");

                cerr << "game: (info) Received \"" << lobby.user_input << "\"\n";

                shipTable [i] [j] = stoi (lobby.user_input);

                if (shipTable [i] [j] != 0) {
                    shipHealth [i] [shipTable [i] [j] - 1] ++;
                }

                dbc.UpdateLobby (lobbyId, "r", lobby.user_input, "", lobby.admin_map, lobby.opponent_map, 0, "w", playerId [i]);

            }

            cerr << "game: (info) Player " << playerId [i] << " map for lobby " << lobbyId << " successfully received\n";
        }

    }



    int currentPlayer = 0; //player 1 starts
    int opponentPlayer = 1; //playing against the next player

    lobby = dbc.ConsoleRead (lobbyId); //update local info before launching the game proper
    dbc.UpdateLobby (lobbyId, "i", lobby.user_input, "", lobby.admin_map, lobby.opponent_map, 0, "n", playerId [currentPlayer]); //Set lobby_status to i before telling server initialisation succeeded to prevent it from launching multiple instances of the same lobby

    cout << "0 " << flush; //Telling server that we have successfully initialised a game (flush force sends a line buffer, otherwise data is only sent when the game finishes)

    //--------------------GAME PROPER--------------------
    ss<<lobbyId;
    int historyId = dbc.CreateHistoryTable ("Lobby " + ss.str(), lobby.adminID, lobby.opponentID, lobby.admin_map, lobby.opponent_map);
 ss.str("");
    int moveId = 0;

    string consoleOutput = "";

    while (true) {

        dbc.UpdateLobby (lobbyId, "i", lobby.user_input, consoleOutput, lobby.admin_map, lobby.opponent_map, historyId, "w", playerId [currentPlayer]); //send backend response to database and tell frontend that game is waiting for its input

        int tileX;
        int tileY;

        if (playerType [currentPlayer] == 0) {
            //frontend
            
            cerr << "game: (info) Waiting for player " << playerId [currentPlayer] << " move\n";

            do { //wait until frontend moves
                lobby = dbc.ConsoleRead (lobbyId);
            } while (lobby.game_status != "c");

            cerr << "game: (info) Player " << playerId [currentPlayer] << " move: \"" << lobby.user_input << "\"\n";

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
                    //current player has won the game

                    dbc.WriteWinner (playerId [currentPlayer], historyId);

                    //write last move to lobby and move tables
                    ss<<shipTable [opponentPlayer] [tableWidth * tileY + tileX];
                    dbc.UpdateLobby (lobbyId, "i", lobby.user_input, "2-" + ss.str(), lobby.admin_map, lobby.opponent_map, historyId, "f", playerId [currentPlayer]);

                    ss.str("");

                    string pseudoInput, pseudoOutput;
                    ss<<tileX + 1;
         pseudoInput = ss.str();
        ss.str("");

        ss<<tileY + 1;
        pseudoInput+=  "-" + ss.str();
        ss.str("");
                    ss<<shipTable [opponentPlayer] [tableWidth * tileY + tileX];
                    pseudoOutput = "2-" + ss.str();

                    ss.str("");

                    dbc.UpdateMoveTable (historyId, moveId, pseudoInput, pseudoOutput, playerId [currentPlayer]);

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

        ss<<response-1;
        string pseudoOutput = ss.str(); //pseudoInput and pseudoOutput are used to send data that has not necessarily been generated to moves table
        ss.str("");
        if (response == 3) {
                ss<<subresponse;
            pseudoOutput = pseudoOutput + "-" + ss.str();
        ss.str("");
        }
        ss<<tileX + 1;
        string pseudoInput = ss.str();
        ss.str("");

        ss<<tileY + 1;
        pseudoInput+=  "-" + ss.str();
        ss.str("");

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


