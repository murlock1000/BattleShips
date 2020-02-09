#include <iostream> //do i really need to tell what this header provides?
#include <bits/stdc++.h> //provides string to char* conversion utility.
#include <string>
#include "stdcomm.h" //Provides AI communication functions prototypes
#include "dbconnector/dbconnector.h" //Provides database communication functions prototypes
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

using namespace std;

void disconnect (int registeredPlayers, int fdOutput[], int fdInput[], int pid[], bool playerType[], DBconnector &dbc, int lobbyId) { //Terminates connections
	
	for (int i = 0; i < registeredPlayers; i ++) {
		if (playerType [i]) { //ai
			stdWrite (fdOutput [i], 4); //Tells AI to exit.
			stdDisconnect (pid [i]); //Kills AI in case it hasn't exited yet
		}
	}

	//deleting lobby

	dbc.InitiateDeletion (lobbyId);

	stdClosePipes (fdInput);
	stdClosePipes (fdOutput);
}

int sendError (DBconnector &dbc, int lobbyId, int timeout, int playerNumber, int fdOutput [], int fdInput [], int pid [], bool playerType [], int registeredPlayers, int playerId [], bool isPlayerConnected []);

int waitForUserResponse (DBconnector &dbc, int lobbyId, int timeout, int playerNumber, int fdOutput [], int fdInput [], int pid [], bool playerType [], int registeredPlayers, int playerId [], bool isPlayerConnected [], int currentPlayer) {
	DBconnector::ConsoleReadStruct lobby;
	int tickCount = timeout * 100;
	do { 
		lobby = dbc.ConsoleRead (lobbyId);
		this_thread::sleep_for(chrono::milliseconds(10));
		tickCount --;
	} while (lobby.game_status != "c" && tickCount > 0);
	if (tickCount == 0) {
		cerr << "game: (error) Request for client timed out, terminating game " << lobbyId << "\n";
		isPlayerConnected [currentPlayer] = 0;
		sendError (dbc, lobbyId, timeout, playerNumber, fdOutput, fdInput, pid, playerType, registeredPlayers, playerId, isPlayerConnected);
		return -1;
	}

	//If user input is required a client is expected to send "moar" request every 5 seconds.
	//Upon receiving this request the server will reset the tick count
	//This is used solely to tell server that the client is still connected and just needs more time.

	else if (lobby.user_input == "moar") {
		cerr << "game: (debug) \"moar\" request received\n";
		dbc.UpdateLobby (lobbyId, "i", lobby.user_input, "", lobby.admin_map, lobby.opponent_map, 0, "w", currentPlayer);
		return waitForUserResponse (dbc, lobbyId, timeout, playerNumber, fdOutput, fdInput, pid, playerType, registeredPlayers, playerId, isPlayerConnected, currentPlayer);
	}
	return 0;
}

int sendError (DBconnector &dbc, int lobbyId, int timeout, int playerNumber, int fdOutput [], int fdInput [], int pid [], bool playerType [], int registeredPlayers, int playerId [], bool isPlayerConnected []) {
	for (int i = 0; i < playerNumber; i ++) {
		if (!playerType [i] && isPlayerConnected [i]) { //if human player is connected
			dbc.UpdateLobby (lobbyId, "i", "", "", "", "", 0, "e", playerId [i]);
			if (waitForUserResponse (dbc, lobbyId, timeout, playerNumber, fdOutput, fdInput, pid, playerType, registeredPlayers, playerId, isPlayerConnected, i) < 0) return -1;
			isPlayerConnected [i] = 0;
		}
	}

	disconnect (registeredPlayers, fdOutput, fdInput, pid, playerType, dbc, lobbyId);

	cerr << "game: (error) Game " << lobbyId << " quit due to an error\n";
	return 0;
}

int finishGame (DBconnector &dbc, int lobbyId, int timeout, int playerNumber, int fdOutput [], int fdInput [], int pid [], bool playerType [], int registeredPlayers, int playerId [], int historyId, bool isPlayerConnected []) {
	for (int i = 0; i < playerNumber; i ++) {
		if (!playerType [i]) {
			dbc.UpdateLobby (lobbyId, "i", "", "", "", "", historyId, "f", playerId [i]);
			if (waitForUserResponse (dbc, lobbyId, timeout, playerNumber, fdOutput, fdInput, pid, playerType, registeredPlayers, playerId, isPlayerConnected, i) < 0) return -1;
		}
	}

	disconnect (registeredPlayers, fdOutput, fdInput, pid, playerType, dbc, lobbyId);

	cerr << "game: (info) Game " << lobbyId << " successfully finished\n";
	return 0;
}

int main (int argc, char* argv []) {
stringstream ss;
	if (argc != 2) { //must have exactly one argument (not counting process name)
		cerr << "game: (error) Must be launched with exactly one argument\n";
		cout << "1 " << flush;
		return 1;
	}

	DBconnector dbc;

	dbc.Connect ("127.0.0.1", "ServerAdmin", "admin", "battleships"); //connecting to database

	int lobbyId = stoi(argv[1]); //reads lobbyId from a provided argument

	DBconnector::ConsoleReadStruct lobby = dbc.ConsoleRead (lobbyId); //gets information about lobby

	dbc.UpdateLobby (lobbyId, "i", lobby.user_input, "", lobby.admin_map, lobby.opponent_map, 0, "n", 0); //Set lobby_status to i before telling server that game launch succeeded to prevent it from launching multiple instances of the same lobby

	cout << "0 " << flush; //Telling server that we have successfully launched a game (flush force sends a line buffer, otherwise data is only sent when the game finishes)

	//May be saved in lobby table later

	int tableWidth = 10;
	int tableHeight = 10;
	int shipNumber = 5;
	int playerNumber = 2;
	int timeout = 30;

	//--------------------INITIALISATION--------------------

	bool playerType [playerNumber];
	int playerId [playerNumber];
	int shipHealth [playerNumber] [shipNumber];
	int shipTable [playerNumber] [tableWidth * tableHeight];
	int shipsLeft [playerNumber];
	int fdInput [playerNumber]; //holds input file descriptors.
	int fdOutput [playerNumber]; //holds output file descriptors.
	int pid [playerNumber]; //holds ai pids
	bool isPlayerConnected [playerNumber];

	//initialising players

	srand (time(NULL));
	int admin = rand() % playerNumber; //randomises which player plays first

	//We should get players' ID first before attempting to communicate with them.
	//This means that if initialisation of any player fails, all clients can be warned about failure.

	for (int i = 0; i < playerNumber; i++) {
		if (i == admin) {
			playerId [i] = lobby.adminID;
		}

		else {
			playerId [i] = lobby.opponentID;
		}
	}

	//Now we can properly initialise players

	for (int i = 0; i < playerNumber; i ++) {

		isPlayerConnected [i] = 1;
		shipsLeft [i] = shipNumber;

		for (int j = 0; j < shipNumber; j++) {
			shipHealth [i] [j] = 0;
		}

		DBconnector::UserInfoTable userInfo; //get info about user

		if (i == admin) {
			userInfo = dbc.GetUserInfo (lobby.adminID);
		}


		else {
			userInfo = dbc.GetUserInfo (lobby.opponentID);
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

				sendError (dbc, lobbyId, timeout, playerNumber, fdOutput, fdInput, pid, playerType, i, playerId, isPlayerConnected);
				return 1;
			}
			else if (stdConnSuccess > 0) { //code executed by child
				return 0;
			}

			pid [i] = *aiPid;
			fdInput [i] = aiIO [0]; //Store each player's fds
			fdOutput [i] = aiIO [1];
		}

		else { //human player initialisation

			dbc.UpdateLobby (lobbyId, "i", lobby.user_input, "", lobby.admin_map, lobby.opponent_map, 0, "w", playerId [i]);

			if (waitForUserResponse (dbc, lobbyId, timeout, playerNumber, fdOutput, fdInput, pid, playerType, i, playerId, isPlayerConnected, i) < 0) return 1;
			lobby = dbc.ConsoleRead (lobbyId);

		}

		//generating map string

		string mapString = "";

		for (int j = 0; j < tableWidth * tableHeight; j++) {
			if (playerType [i]) {
				shipTable [i] [j] = stdRead (fdInput [i]); //reading ship table from ai
			}
			else {
				if (waitForUserResponse (dbc, lobbyId, timeout, playerNumber, fdOutput, fdInput, pid, playerType, i, playerId, isPlayerConnected, i) < 0) return 1;
				lobby = dbc.ConsoleRead (lobbyId);

				shipTable [i] [j] = stoi (lobby.user_input);
				dbc.UpdateLobby (lobbyId, "i", lobby.user_input, "", lobby.admin_map, lobby.opponent_map, 0, "w", playerId [i]);
			}
			ss<<shipTable [i] [j];

			mapString += ss.str();
			ss.str("");
			if (shipTable [i] [j] != 0) {
				shipHealth [i] [shipTable [i] [j] - 1] ++;
			}

		}
		
		if (i == admin) {
			dbc.UpdateLobby (lobbyId, "i", lobby.user_input, "", mapString, lobby.opponent_map, 0, "n", 0);
		}
		else {
			dbc.UpdateLobby (lobbyId, "i", lobby.user_input, "", lobby.admin_map, mapString, 0, "n", 0);
		}

		lobby = dbc.ConsoleRead (lobbyId); //updating local information to prevent overwriting ship tables

	}



	int currentPlayer = 0; //player 1 starts
	int opponentPlayer = 1; //playing against the next player

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
			
			if (waitForUserResponse(dbc, lobbyId, timeout, playerNumber, fdOutput, fdInput, pid, playerType, playerNumber, playerId, isPlayerConnected, currentPlayer) < 0) return 1; //Waiting for frontend to aknowledge result
			lobby = dbc.ConsoleRead (lobbyId);
			
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

		int response; //tells move consequences (missed, hit or sunk a ship)
		int subresponse; //tells which ship was sunk (only used when a ship was sunk)

		if (shipTable [opponentPlayer] [tableWidth * tileY + tileX] == 0) { //Tile isn't taken by any ship

			response = 1; //missed

		}

		else {

			if (--shipHealth [opponentPlayer] [shipTable [opponentPlayer] [tableWidth * tileY + tileX] - 1] == 0) { //Target tile is the last tile of some ship

				shipsLeft [opponentPlayer]--;

				if (shipsLeft [opponentPlayer] == 0) { //Game over

					//write last move to lobby table
					ss<<shipTable [opponentPlayer] [tableWidth * tileY + tileX];
					dbc.UpdateLobby (lobbyId, "i", lobby.user_input, "2-" + ss.str(), lobby.admin_map, lobby.opponent_map, historyId, "w", playerId [currentPlayer]);
					if (waitForUserResponse(dbc, lobbyId, timeout, playerNumber, fdOutput, fdInput, pid, playerType, playerNumber, playerId, isPlayerConnected, currentPlayer) < 0) return 1;

					//current player has won the game

					dbc.WriteWinner (playerId [currentPlayer], historyId);

					//write last move to move table
					ss.str("");

					string pseudoInput, pseudoOutput;
					ss<<tileX;
					pseudoInput = ss.str();
					ss.str("");

					ss<<tileY;
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
		ss<<tileX;
		string pseudoInput = ss.str();
		ss.str("");

		ss<<tileY;
		pseudoInput+=  "-" + ss.str();
		ss.str("");

		dbc.UpdateMoveTable (historyId, moveId, pseudoInput, pseudoOutput, playerId [currentPlayer]);

		if (playerType [currentPlayer] == 0) {
			//send console response to lobby table
			consoleOutput = pseudoOutput;
			dbc.UpdateLobby (lobbyId, "i", lobby.user_input, consoleOutput, lobby.admin_map, lobby.opponent_map, historyId, "w", playerId [currentPlayer]); //send backend response to database
			if (waitForUserResponse(dbc, lobbyId, timeout, playerNumber, fdOutput, fdInput, pid, playerType, playerNumber, playerId, isPlayerConnected, currentPlayer) < 0) return 1; //Waiting for frontend to aknowledge result
			lobby = dbc.ConsoleRead (lobbyId);
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

	//tell all players that game is now finished

	if (finishGame (dbc, lobbyId, timeout, playerNumber, fdOutput, fdInput, pid, playerType, playerNumber, playerId, historyId, isPlayerConnected) < 0) return 1;

	return 0;
}


