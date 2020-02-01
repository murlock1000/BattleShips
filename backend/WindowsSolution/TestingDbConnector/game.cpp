#include "pch.h"
#include <iostream> //do i really need to tell what this header provides?
//#include <bits/stdc++.h> //provides string to char* conversion utility.
#include <string>
#include "stdcomm.h" //Provides AI communication functions prototypes
#include "dbconnector.h"
//#include "dbconnector/dbconnector.h" //Provides database communication functions prototypes
#include <cstdlib>
#include <ctime>

//Windows compatibility

#ifdef __linux__
#define HANDLE int
#endif

using namespace std;

int disconnect(int playerNumber, int activadedAis, HANDLE fdOutput[], HANDLE fdInput[], int pid[], bool playerType[], int winnerID, DBconnector& dbc, DBconnector::ConsoleReadStruct& lobby, int lobbyId, int historyId, int playerId[]) { //Terminates connections
	int success = 0;
	int timeout;
	bool isNull;
	if (winnerID == NULL) {
		isNull = true;
	}
	else {
		isNull = false;
	}
	//cout << "end " << isNull << endl;

	if (winnerID != NULL) {
		dbc.WriteWinner(winnerID, historyId);
		for (int i = 0; i < playerNumber; i++) {
			if (playerType[i]) { //ai
				stdWrite(fdOutput[i], 4); //Tells AI to exit.
				if (stdDisconnect(pid[i]) < 0) { //Kills AI in case it hasn't exited yet
					success = -1;
				}
				dbc.AcknowledgeEnd(lobbyId, playerId[i], 0);
			}
			else { //frontend
				dbc.UpdateLobby(lobbyId, "i", "", "", "", "", historyId, "f", playerId[i]);
				timeout = 500;
				do { //wait until frontend moves
					lobby = dbc.ConsoleRead(lobbyId);
					Sleep(10);
					timeout--;
				} while (lobby.game_status != "c" && timeout > 0);

				//to be implemented.
			}
		}
	}
	else {
		for (int i = 0; i < playerNumber; i++) {
			if (i < activadedAis) {
				if (playerType[i]) { //ai
					stdWrite(fdOutput[i], 4); //Tells AI to exit.
					if (stdDisconnect(pid[i]) < 0) { //Kills AI in case it hasn't exited yet
						success = -1;
					}
					dbc.AcknowledgeEnd(lobbyId, playerId[i], 0);
				}
			}
			else {
				dbc.UpdateLobby(lobbyId, "i", "", "", "", "", historyId, "e", playerId[i]);
				timeout = 500;
				do { //wait until frontend moves
					lobby = dbc.ConsoleRead(lobbyId);
					Sleep(10);
					timeout--;
				} while (lobby.game_status != "c" && timeout > 0);
			}
		}
	}

	dbc.InitiateDeletion(lobbyId);

	return success;
}

int main(int argc, char* argv[]) {

	/*  if (argc != 2) { //must have exactly one argument (not counting process name)
		  cerr << "game: Must be launched with exactly one argument\n";
		  return 1;
	  }
	  */
	DBconnector dbc;
	dbc.Connect("127.0.0.1", "root", "root", "battleships"); //connecting to database

	//int lobbyId = stoi(argv[1]); //reads lobbyId from a provided argument
	int lobbyId; //debugging purposes
	cin >> lobbyId;
	DBconnector::ConsoleReadStruct lobby;

	lobby = dbc.ConsoleRead(lobbyId); //gets information about lobby

	//cout << lobby.adminID << " a " << lobby.admin_map << " b " << lobby.game_status << " c " << lobby.opponentID << " d " << lobby.opponent_map << " e " << lobby.user_input << endl;

	//May be saved in lobby table later

	int tableWidth = 10;
	int tableHeight = 10;
	int shipNumber = 5;
	int playerNumber = 2;
	int intOut;

	//--------------------INITIALISATION--------------------

	bool* playerType = new bool[playerNumber];
	int* playerId = new int[playerNumber];
	int** shipHealth = new int* [playerNumber];
	for (int i = 0; i < playerNumber; i++) {
		shipHealth[i] = new int[shipNumber];
	}
	int** shipTable = new int* [playerNumber];
	for (int i = 0; i < playerNumber; i++) {
		shipTable[i] = new int[tableWidth * tableHeight];
	}
	int* shipsLeft = new int[playerNumber];
	HANDLE* fdInput = new HANDLE[playerNumber]; //holds input file descriptors.
	HANDLE* fdOutput = new HANDLE[playerNumber]; //holds output file descriptors.
	int* pid = new int[playerNumber]; //holds ai pids

	//initialising players

	srand(time(NULL));
	int admin = rand() % playerNumber; //randomises which player plays first

	//////////////////////////////////////TESTING
	/*
	string aiName = "test";

	string aiProcName = aiName + ".exe";
	string aiPath = "./ai/" + aiProcName;
  //  cout  << " name: " << aiName << endl;
	cout << aiPath  << endl;
	HANDLE aiIO[2];

	int* aiPid = new int;

	int stdConnSuccess = stdConnect(aiIO, aiPid, aiPath.c_str(), aiProcName.c_str(), "0");

	if (stdConnSuccess < 0) {
		disconnect(0, fdOutput, fdInput, pid, playerType);
		cout << "failed to initialise "; //tells server game initialisation failed
		return 1;
	}
	else if (stdConnSuccess > 0) { //code executed by child
		return 0;
	}

	pid[0] = *aiPid;
	fdInput[0] = aiIO[0]; //Store each player's fds
	fdOutput[0] = aiIO[1];

	//generating ai ship string which will be sent to history and lobby tables

	string aiShipTable = "";

	stdRead(fdInput[0]);
	stdRead(fdInput[0]);
	stdRead(fdInput[0]);
	stdRead(fdInput[0]);
	stdRead(fdInput[0]);
	stdRead(fdInput[0]);

  //  for (int j = 0; j < tableWidth * tableHeight; j++) {
	 //   shipTable[0][j] = stdRead(fdInput[0]); //reading ship table from ai				//MUST FIX ASYNCHRONOUS COUT!!!!!!!!!!!!!!!!!!!!!!!!!!!! stdRead does not read int by int, but reads the whole outputed buffer...
		//cout << shipTable[0][j] << " ";
	  //  aiShipTable += to_string(shipTable[i][j]);

  //  }
 //   cout << endl;
 */

	int winnerID;
	int timeout = 3100;

	for (int i = 0; i < playerNumber; i++) {
		shipsLeft[i] = shipNumber;

		for (int j = 0; j < shipNumber; j++) {
			shipHealth[i][j] = 0;
		}

		DBconnector::UserInfoTable userInfo; //get info about user

		if (i == admin) {

			userInfo = dbc.GetUserInfo(lobby.adminID);

			playerId[i] = lobby.adminID;
		}

		else {

			userInfo = dbc.GetUserInfo(lobby.opponentID);

			playerId[i] = lobby.opponentID;
		}

		playerType[i] = userInfo.is_ai;

		//cout << "playerType " << playerType[i] << endl;

		if (playerType[i]) { //ai initialisation

			string aiName = userInfo.username;

			string aiProcName = aiName + ".exe";
			string aiPath = "./ai/" + aiProcName;
			//	cout << "playerType: " << playerType[i] << " name: " << aiName << endl;
			//	cout << aiPath << " ph" << endl;
			HANDLE aiIO[2];

			int* aiPid = new int;

			int stdConnSuccess = stdConnect(aiIO, aiPid, aiPath.c_str(), aiProcName.c_str(), "0");

			if (stdConnSuccess < 0) {
				//If launching an ai process fails, we have to kill parent.
				//However, if there are active players waiting for the ai to start, they have to be informed
				//that the game has ended.

				disconnect(playerNumber, i, fdOutput, fdInput, pid, playerType, NULL, dbc, lobby, lobbyId, NULL, playerId);
				cout << "1 "; //tells server game initialisation failed
				return 1;
			}
			else if (stdConnSuccess > 0) { //code executed by child ????
				return 0;
			}

			pid[i] = *aiPid;
			fdInput[i] = aiIO[0]; //Store each player's fds
			fdOutput[i] = aiIO[1];

			//generating ai ship string which will be sent to history and lobby tables
			intOut = stdRead(fdInput[i]);
			if (intOut != 0) {
				cout << "AI DID NOT START CORRECTLY" << endl;
				disconnect(playerNumber, i, fdOutput, fdInput, pid, playerType, NULL, dbc, lobby, lobbyId, NULL, playerId);
				//cout << "a" << endl;
				return -1;

			}
			string aiShipTable = "";
			//	cout << "aitable: " << endl;
			for (int j = 0; j < tableWidth * tableHeight; j++) {
				intOut = stdRead(fdInput[i]); //store AI's response in a temporary integer for error checking
				//cout << intOut << " ";
				if (intOut == -1) {
					disconnect(playerNumber, i, fdOutput, fdInput, pid, playerType, NULL, dbc, lobby, lobbyId, NULL, playerId);
					//cout << "a" << endl;
					return -1;
				}
				else {
					shipTable[i][j] = intOut; //reading ship table from ai	
							 //   cout << shipTable[i][j] << " ";
					aiShipTable += to_string(shipTable[i][j]);

					if (shipTable[i][j] != 0) {
						shipHealth[i][shipTable[i][j] - 1] ++;
					}
				}
			}
			lobby = dbc.ConsoleRead(lobbyId); //updating local information to prevent overwriting ship tables

			if (i == admin) {
				dbc.UpdateLobby(lobbyId, "r", lobby.user_input, "", aiShipTable, lobby.opponent_map, 0, "n", 0);
			}
			else {
				dbc.UpdateLobby(lobbyId, "r", lobby.user_input, "", lobby.admin_map, aiShipTable, 0, "n", 0);
			}

		}

		else { //human player initialisation


			string userShipTable = "";
			lobby = dbc.ConsoleRead(lobbyId); //updating local information to prevent overwriting ship tables

			//converting ship string to a ship table
			//cout << "cia: "<< userShipTable << endl;
			for (int j = 0; j < tableWidth * tableHeight; j++) {

				dbc.UpdateLobby(lobbyId, "r", "", "", lobby.admin_map, lobby.opponent_map, 0, "w", playerId[i]);

				timeout = 3100;//15seconds until the opponent wins
				do { //wait until frontend moves
					lobby = dbc.ConsoleRead(lobbyId);
					Sleep(5);
					timeout--;
				} while (lobby.game_status != "c" && timeout > 0);

				if (timeout == 0) { //opponent wins if user/ his AI does not make a turn in 30 seconds
					disconnect(playerNumber, playerNumber, fdOutput, fdInput, pid, playerType, NULL, dbc, lobby, lobbyId, NULL, playerId);
					return -1;
				}
				//read user's move
				try
				{
					//cout <<"userInput: "<< lobby.user_input << endl;
					shipTable[i][j] = stoi(lobby.user_input); //try to convert string to int
					userShipTable += to_string(shipTable[i][j]);
					//cout << to_string(shipTable[i][j]) << " "; --SHOWCASING INPUT
				}
				catch (const std::exception&)
				{
					disconnect(playerNumber, i, fdOutput, fdInput, pid, playerType, NULL, dbc, lobby, lobbyId, NULL, playerId); //if input is invalid - close the game
					return -1;
				}

				if (shipTable[i][j] != 0) {
					shipHealth[i][shipTable[i][j] - 1] ++;
				}
			}

			//cout << endl;

			lobby = dbc.ConsoleRead(lobbyId); //updating local information to prevent overwriting ship tables

			if (i == admin) {
				dbc.UpdateLobby(lobbyId, "r", lobby.user_input, "", userShipTable, lobby.opponent_map, 0, "n", 0);
			}
			else {
				dbc.UpdateLobby(lobbyId, "r", lobby.user_input, "", lobby.admin_map, userShipTable, 0, "n", 0);
			}
		}


	}

	int currentPlayer = 0; //player 1 starts
	int opponentPlayer = 1; //playing against the next player

	lobby = dbc.ConsoleRead(lobbyId); //update local info before launching the game proper
	dbc.UpdateLobby(lobbyId, "i", "", "", lobby.admin_map, lobby.opponent_map, 0, "n", playerId[currentPlayer]); //Set lobby_status to i before telling server initialisation succeeded to prevent it from launching multiple instances of the same lobby

	cout << "0 " << flush; //Telling server that we have successfully initialised a game (flush force sends a line buffer, otherwise data is only sent when the game finishes)

	/*
	cout << endl;
	cout << "ai map: " << endl;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			cout << lobby.admin_map[i * 10 + j] << " ";
		}
		cout << endl;
	}
	cout << "player map: " << endl;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			cout << lobby.opponent_map[i * 10 + j] << " ";
		}
		cout << endl;
	}
	*/
	//--------------------GAME PROPER--------------------

	int historyId = dbc.CreateHistoryTable("Lobby " + to_string(lobbyId), lobby.adminID, lobby.opponentID, lobby.admin_map, lobby.opponent_map);
	//cout << "historyID: " << historyId << endl;
	int moveId = 0;
	string consoleOutput = "";
	string enemyMove = "";
	while (true) {

		dbc.UpdateLobby(lobbyId, "i", "", enemyMove, lobby.admin_map, lobby.opponent_map, historyId, "w", playerId[currentPlayer]); //send the previous players move to the other player and wait for his action

		int tileX;
		int tileY;

		if (playerType[currentPlayer] == 0) {
			//frontend
			timeout = 3100;//30seconds until the opponent wins
			do { //wait until frontend moves
				lobby = dbc.ConsoleRead(lobbyId);
				Sleep(10);
				timeout--;
			} while (lobby.game_status != "c" && timeout > 0);

			if (timeout == 0) { //opponent wins if user/ his AI does not make a turn in 30 seconds
				disconnect(playerNumber, playerNumber, fdOutput, fdInput, pid, playerType, playerId[currentPlayer], dbc, lobby, lobbyId, historyId, playerId);
				//	cout << "b" << endl;
				return -1;
			}
			//read user's move

			int dashPosition = 0;
			enemyMove = lobby.user_input;
			while (lobby.user_input.substr(dashPosition, 1) != "-") dashPosition++;
			try
			{
				tileX = stoi(lobby.user_input.substr(0, dashPosition));
				tileY = stoi(lobby.user_input.substr(dashPosition + 1, lobby.user_input.length() - 1));
			}
			catch (const std::exception&)
			{
				disconnect(playerNumber, playerNumber, fdOutput, fdInput, pid, playerType, playerId[currentPlayer], dbc, lobby, lobbyId, historyId, playerId); //if players output is invalid - he loses
				return -1;
			}

		//	cout << "player move: " << tileX << "-" << tileY << endl;
		}
		else {
			//ai
			intOut = stdRead(fdInput[currentPlayer]);
			if (intOut == -1) {
				disconnect(playerNumber, playerNumber, fdOutput, fdInput, pid, playerType, playerId[currentPlayer], dbc, lobby, lobbyId, historyId, playerId);
				return -1;
			}
			else {
				tileX = intOut;
			}

			intOut = stdRead(fdInput[currentPlayer]);
			if (intOut == -1) {
				disconnect(playerNumber, playerNumber, fdOutput, fdInput, pid, playerType, playerId[currentPlayer], dbc, lobby, lobbyId, historyId, playerId);
				return -1;
			}
			else {
				tileY = intOut;
			}

			enemyMove = to_string(tileX) + "-" + to_string(tileY);

				//cout << "ai move: " << enemyMove << endl;

		}

		int response; //tells move consequences (missed, hit or sunk a ship)
		int subresponse; //tells which ship was sunk (only used when a ship was sunk)


		if (shipTable[opponentPlayer][tableWidth * tileY + tileX] == 0) { //Tile isn't taken by any ship

			response = 1; //missed

		}
		else {

			if (--shipHealth[opponentPlayer][shipTable[opponentPlayer][tableWidth * tileY + tileX] - 1] == 0) { //Target tile is the last tile of some ship

				shipsLeft[opponentPlayer]--;

				if (shipsLeft[opponentPlayer] == 0) { //Game over
					//current player has won the game

				//	cout << "GAME OVER" << endl;

					winnerID = playerId[currentPlayer];

					//write last move to lobby and move tables
					dbc.UpdateLobby(lobbyId, "i", lobby.user_input, "2-" + to_string(shipTable[opponentPlayer][tableWidth * tileY + tileX]), lobby.admin_map, lobby.opponent_map, historyId, "w", playerId[currentPlayer]);

					if (playerType[currentPlayer] == 0) {

						timeout = 3100;//30seconds until the opponent wins
						do { //wait until frontend moves
							lobby = dbc.ConsoleRead(lobbyId);
							Sleep(10);
							timeout--;
						} while (lobby.game_status != "c" && timeout > 0);
					}

					string pseudoInput, pseudoOutput;
					pseudoInput = to_string(tileX + 1) + "-" + to_string(tileY + 1);
					pseudoOutput = "2-" + to_string(shipTable[opponentPlayer][tableWidth * tileY + tileX]);
					dbc.UpdateMoveTable(historyId, moveId, pseudoInput, pseudoOutput, playerId[currentPlayer]);

					break;
				}

				//not game over; ship sunk

				response = 3;

				subresponse = shipTable[opponentPlayer][tableWidth * tileY + tileX];

			}

			else { //Tile is taken by a ship, but it isn't its last tile

				response = 2; //ship hit

			}

		}


		string pseudoOutput = to_string(response - 1); //pseudoInput and pseudoOutput are used to send data that has not necessarily been generated to moves table
		if (response == 3) {
			pseudoOutput = pseudoOutput + "-" + to_string(subresponse);
		}

		string pseudoInput = to_string(tileX) + "-" + to_string(tileY);

		dbc.UpdateMoveTable(historyId, moveId, pseudoInput, pseudoOutput, playerId[currentPlayer]);

		if (playerType[currentPlayer] == 0) {
			//send console response to lobby table
			consoleOutput = pseudoOutput;
			dbc.UpdateLobby(lobbyId, "i", lobby.user_input, consoleOutput, lobby.admin_map, lobby.opponent_map, historyId, "w", playerId[currentPlayer]); //returning the result of an action
			//cout << "result: " <<consoleOutput<<endl;

			timeout = 3100;//30seconds until the opponent wins
			do { //wait until frontend moves
				lobby = dbc.ConsoleRead(lobbyId);
				Sleep(10);
				timeout--;
			} while (lobby.game_status != "c" && timeout > 0);

			if (timeout == 0) { //opponent wins if user/ his AI does not make a turn in 30 seconds
				disconnect(playerNumber, playerNumber, fdOutput, fdInput, pid, playerType, playerId[currentPlayer], dbc, lobby, lobbyId, historyId, playerId);
				//	cout << "b" << endl;
				return -1;
			}
		}
		else {
			//send console response directly to ai
			stdWrite(fdOutput[currentPlayer], response);
		}

		//   cout << "response: " << response <<"0"<< endl;

		currentPlayer = (currentPlayer + 1) % playerNumber; //Next player's turn
		opponentPlayer = (currentPlayer + 1) % playerNumber;

		moveId++;

	}

	//--------------------DISCONNECTION--------------------

	//disconnecting players
	//cout << "disconnecting" << endl;

	disconnect(playerNumber, playerNumber, fdOutput, fdInput, pid, playerType, winnerID, dbc, lobby, lobbyId, historyId, playerId);
	//cout << "e" << endl;
	//deleting lobby

	return 0;
}
