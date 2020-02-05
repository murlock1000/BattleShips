#include "dbconnector.h"
#include <mysql.h>
#include <iostream>
#include <sstream>
#include <cppconn/statement.h>
#include <vector>
using namespace std;

DBconnector::DBconnector()
{

}

DBconnector::~DBconnector()
{
}

///userside functions
void DBconnector::Connect(string ip, string username, string pass, string database)		// connects to database with user credentials. requires database ip, username, password, database name.
{
	conn = mysql_init(0);

	conn = mysql_real_connect(conn, ip.c_str(), username.c_str(), pass.c_str(), database.c_str(), 3306, NULL, 0);
	if (conn)
	{
		//cout << "Connected!" << endl;
	}
	else {
		throw "Failed To connect";
	}
}

int DBconnector::Register(std::string username)		//creates a new user in database if username is unique, performs a login if successful.
{
	query = "INSERT INTO users(username,is_ai) VALUES('" + username + "',0)";
	if (PassQuery(query) != 0) {
		throw mysql_error(conn);
	}

	return Login(username);
}

int DBconnector::Login(string username)		//after passing a registered username returns the userID used for other functions.
{
	query = "SELECT userID FROM users WHERE username='" + username + "'";

	if (PassQuery(query) != 0) {
		throw mysql_error(conn);
	}
	MYSQL_RES *result = mysql_store_result(conn);

	if (result == NULL) {
		throw mysql_error(conn);
	}

	MYSQL_ROW row = mysql_fetch_row(result);

	if (row == NULL) {
		throw "No such user";
	}

	mysql_free_result(result);				//must be done after every SELECT query

	return atoi(row[0]);					//converts char* to int

	//cout << typeid(row[0]).name() << endl; — shows the datatype of a variable
}

void DBconnector::ChangeName(string newName, int userID)	//changes the username of the userID to a new one (can't change into an occupied username).
{
	ss << userID;
	query = "UPDATE users SET username='" + newName + "' WHERE userID=" + ss.str();
	ss.clear();
	ss.str(string());
	if (PassQuery(query) != 0) {
		throw mysql_error(conn);
	}
}

DBconnector::UserInfoTable DBconnector::GetUserInfo(int userID)		//returns the user's username, win and loose count in a UserInfoTable struct.
{
	ss << userID;
	int wins, losses;
    bool is_ai;
	query = "SELECT username, wins, losses, is_ai FROM users WHERE userID=" + ss.str();

    if (PassQuery (query) != 0) {
        throw mysql_error(conn);
    }

	ss.clear();
	ss.str(string());
	MYSQL_RES *result = mysql_store_result(conn);

	if (result == NULL) {
		throw mysql_error(conn);
	}

	MYSQL_ROW row = mysql_fetch_row(result);
	UserInfoTable userInfo{row[0], atoi(row[1]), atoi(row[2]), atoi(row[3])};

	mysql_free_result(result);

	return userInfo;
}

int DBconnector::GetLobbyID(int userID) {	//get the lobbyID which the user is currently in.		(if used in this .cpp must be in a try catch throw)
	ss << userID;

	int lobbyID;
	query = "SELECT current_lobbyID FROM users WHERE userID=" + ss.str();

	ss.clear();
	ss.str(string());

	if (PassQuery(query) != 0) {
		throw mysql_error(conn);
	}
	MYSQL_RES *result = mysql_store_result(conn);

	if (result == NULL) {
		throw mysql_error(conn);
	}

	MYSQL_ROW row = mysql_fetch_row(result);

	if (row[0] == NULL) {
		lobbyID = -1;
	}
	else {
		lobbyID = atoi(row[0]);
	}

	mysql_free_result(result);

	return lobbyID;
}

void DBconnector::SetUserLobby(int userID, int lobbyID) {	//sets the lobbyID in the user table (used after connecting to a lobby)
	ss << userID;
	string uID = ss.str();
	ss.clear();
	ss.str(string());
	ss << lobbyID;
	query = "UPDATE users SET current_lobbyID=" + ss.str() + " WHERE userID=" + uID;
	ss.clear();
	ss.str(string());
	if (PassQuery(query) != 0) {
		throw mysql_error(conn);
	}
}

int DBconnector::CreateLobby(string lobbyName, int userID)	//1.Check if player is already in a lobby (if true, returns lobbyID) 2.Create a lobby 3.Update current_lobbyID in user table 4. return lobbyID
{
	int cLobby;
	try
	{
		cLobby = GetLobbyID(userID);
	}
	catch (const char* e)
	{
		throw e;
	}

	if (cLobby != -1) {
		return cLobby;		//if a player is in a lobby return the lobbyID
	}

	ss << userID;
	query = "INSERT INTO lobbies(lobby_name,adminID) VALUES('" + lobbyName + "'," + ss.str() + ")";
	ss.clear();
	ss.str(string());
	if (PassQuery(query) != 0) {
		throw mysql_error(conn);
	}

	query = "SELECT LAST_INSERT_ID()";	//get last inserted row index

	if (PassQuery(query) != 0) {
		throw mysql_error(conn);
	}

	MYSQL_RES *result = mysql_store_result(conn);

	if (result == NULL) {
		throw mysql_error(conn);
	}

	MYSQL_ROW row = mysql_fetch_row(result);

	cLobby = atoi(row[0]);
	mysql_free_result(result);
	try
	{
		SetUserLobby(userID, cLobby);
	}
	catch (const char* e)
	{
		throw e;
	}

	return atoi(row[0]);
}

void  DBconnector::LeaveLobby(int lobbyID, int userID)	//if user created a lobby and no one has joined him he can leave it (1. checks if lobby_status!='w' 2. deletes the lobby 3. changes player's lobbyID to -1)
{
	ss << lobbyID;
	query = "SELECT lobby_status FROM lobbies WHERE lobbyID=" + ss.str();

	if (PassQuery(query) != 0) {
		throw mysql_error(conn);
	}
	MYSQL_RES *result = mysql_store_result(conn);

	if (result == NULL) {
		throw mysql_error(conn);
	}

	MYSQL_ROW row = mysql_fetch_row(result);

	cout << "before: " << row[0] << endl;
	if (row[0][0] != 'w') {
		throw "Already in game";
	}
	mysql_free_result(result);

	query = "DELETE FROM lobbies WHERE lobbyID=" + ss.str();
	ss.clear();
	ss.str(string());
	if (PassQuery(query) != 0) {
		throw mysql_error(conn);
	}

	try
	{
		SetUserLobby(userID, -1);
	}
	catch (const char* e)
	{
		throw e;
	}

}

vector<DBconnector::LobbyTable> DBconnector::ListLobbies(bool isUser)	//Returns lobby list. If user is a spectator (isUser=0) returns lobbyID, lobbyName, adminUsername, opponentUsername. If user is a player does not return opponentUsername.
{
	vector<LobbyTable> lobbyTable;
	if (isUser) {
		query="SELECT l.lobbyID, l.lobby_name, u1.username FROM lobbies AS l JOIN users AS u1 ON u1.userID=l.adminID WHERE l.lobby_status='w'";
	}
	else
	{
		query = "SELECT l.lobbyID, l.lobby_name, u1.username, u2.username FROM lobbies AS l JOIN users AS u1 ON u1.userID=l.adminID LEFT JOIN users AS u2 ON u2.userID=l.opponentID WHERE l.lobby_status!='w'";
	}
	if (PassQuery(query) != 0) {
		throw mysql_error(conn);
	}
	MYSQL_RES *result = mysql_store_result(conn);

	if (result == NULL) {
		throw mysql_error(conn);
	}

	MYSQL_ROW row;
	int rowCount = result->row_count;	//get number of lobbies

	for (int i = 0; i < rowCount; i++) {
		row = mysql_fetch_row(result);
		LobbyTable tempLobby;
		tempLobby.lobbyID = atoi(row[0]);
		tempLobby.lobbyName = row[1];
		tempLobby.adminName = row[2];
		if (!isUser) {
			tempLobby.opponentName = row[3];
		}
		lobbyTable.push_back(tempLobby);
	}

	mysql_free_result(result);

	return lobbyTable;
}

void DBconnector::JoinLobbyAsPlayer(int lobbyID, int userID)	//checks if lobby is still available, sets opponentID in the lobby as userID, updates lobbyID in user table.
{
	ss << userID;
	string uID = ss.str();
	ss.clear();
	ss.str(string());
	ss << lobbyID;
	//cout << "lobbyID: " <<uID<<" "<< ss.str()<<endl;
	query = "SELECT lobby_status FROM lobbies WHERE lobbyID=" + ss.str();

	if (PassQuery(query) != 0) {
		throw mysql_error(conn);
	}
	MYSQL_RES *result = mysql_store_result(conn);

	if (result == NULL) {
		throw mysql_error(conn);
	}

	MYSQL_ROW row = mysql_fetch_row(result);

	if (row[0][0] != 'w') {
		mysql_free_result(result);
		throw "Game has already started";
	}
	mysql_free_result(result);

	query = "UPDATE lobbies SET opponentID="+uID+", lobby_status='r' WHERE lobbyID=" + ss.str();
	ss.clear();
	ss.str(string());

	if (PassQuery(query) != 0) {
		throw mysql_error(conn);
	}
	try {
		SetUserLobby(userID, lobbyID);
	}
	catch (const char* e)
	{
		throw e;
	}
}

DBconnector::Rlobby DBconnector::ReadLobby(int lobbyID)	//returns Rlobby struct with game_status, console_output, current_player.
{
	//if game_status==w & current_player==userID -> WriteMove &&  read the outcome of previous action.
	//if game_status==f & current_player==userID -> AcknowledgeEnd


	ss << lobbyID;
	query = "SELECT game_status, console_output, current_player FROM lobbies WHERE lobbyID="+ss.str();
	ss.clear();
	ss.str(string());

	if (PassQuery(query) != 0) {
		throw mysql_error(conn);
	}
	MYSQL_RES* result = mysql_store_result(conn);
	if (result == NULL) {
		throw mysql_error(conn);
	}

	MYSQL_ROW row = mysql_fetch_row(result);
	Rlobby rlobby;

	rlobby.game_status = row[0];
	if (row[1] != NULL) {
		rlobby.console_output = row[1];
	}
	if (row[2] != NULL) {
		rlobby.curr_player = atoi(row[2]);
	}
	
	return rlobby;
}

pair<string, string> DBconnector::ReadMap(int lobbyID)	//returns both maps in a lobby as a pair<string, string> (used for spectating). when using always check if pair.first=="finished" -> pair.second = gameID (for history table) and stop ReadingMap (game has finished).
{
	ss << lobbyID;
	string map1, map2, gameStatus;
	query = "SELECT admin_map, opponent_map, game_status FROM lobbies WHERE lobbyID=" + ss.str();
	ss.clear();
	ss.str(string());
	if (PassQuery(query) != 0) {
		throw mysql_error(conn);
	}
	MYSQL_RES* result = mysql_store_result(conn);

	if (result == NULL) {
		throw mysql_error(conn);
	}

	MYSQL_ROW row = mysql_fetch_row(result);

	if(row[0]!=NULL){
		map1 = row[0];
	}
	
	if(row[0]!=NULL){
		map2 = row[1];
	}

	gameStatus = row[2];

	mysql_free_result(result);

	if (gameStatus == "f") {	//checks if game has finished

		try
		{
			gameStatus = AcknowledgeEnd(lobbyID, -1, 0);
		}
		catch (const char* e)
		{
			throw e;
		}

		return make_pair("finished", gameStatus);
	}

	return make_pair(map1, map2);
}

void DBconnector::WriteMove(int lobbyID, string userIn)	//passes user's input string.
{
	ss << lobbyID;
	query = "UPDATE lobbies SET user_input='" + userIn + "', game_status='c' WHERE lobbyID=" + ss.str();
	ss.clear();
	ss.str(string());

	if (PassQuery(query) != 0) {
		throw mysql_error(conn);
	}
}

int DBconnector::AcknowledgeEnd(int lobbyID, int userID, bool isUser)	//leaves the lobby, updates lobbyID in user table, acknowledges end (if not spectating), returns gameID (for history table)
{
	int historyID;
	ss << lobbyID;
	query = "SELECT historyID FROM lobbies WHERE lobbyID=" + ss.str(); //get gameID to history table

	if (PassQuery(query) != 0) {
		throw mysql_error(conn);
	}
	MYSQL_RES* result = mysql_store_result(conn);

	if (result == NULL) {
		throw mysql_error(conn);
	}

	MYSQL_ROW row = mysql_fetch_row(result);

	historyID = atoi(row[0]);

	mysql_free_result(result);

	if (!isUser) {
		query = "UPDATE lobbies SET user_input='OK', game_status='c' WHERE lobbyID=" + ss.str();

		if (PassQuery(query) != 0) {
			throw mysql_error(conn);
		}
	}

	try
	{
		SetUserLobby(userID, -1);
	}
	catch (const char* e)
	{
		throw e;
	}
	ss.clear();
	ss.str(string());

	return historyID;
}

string DBconnector::GetWinner(int gameID)	//returns winner's username after passing gameID
{
	string username;
	ss << gameID;
	query = "SELECT username FROM users WHERE userID= SELECT winnerID FROM history WHERE gameID ="+ss.str();
	ss.clear();
	ss.str(string());

	if (PassQuery(query) != 0) {
		throw mysql_error(conn);
	}
	MYSQL_RES* result = mysql_store_result(conn);

	if (result == NULL) {
		throw mysql_error(conn);
	}

	MYSQL_ROW row = mysql_fetch_row(result);

	username = row[0];

	return username;
}

DBconnector::History DBconnector::GetInfoOnGame(int gameID)	//returns History struct when passing gameID
{
	History ist;
	ss << gameID;
	query = "SELECT game_name, player1_ID, player2_ID, map1, map2, winnerID FROM history WHERE gameID=" + ss.str();

	MYSQL_RES* result = mysql_store_result(conn);

	if (result == NULL) {
		throw mysql_error(conn);
	}

	MYSQL_ROW row = mysql_fetch_row(result);

	ist.game_name = row[0];
	ist.player1_ID = atoi(row[1]);
	ist.player2_ID = atoi(row[2]);
	ist.map1 = row[3];
	ist.map2 = row[4];
	ist.winnerID = atoi(row[5]);

	mysql_free_result(result);

	query = "SELECT move_pos, move_res, playerID FROM moves WHERE gameID=" + ss.str();

	if (result == NULL) {
		throw mysql_error(conn);
	}

	result = mysql_store_result(conn);

	if (result == NULL) {
		throw mysql_error(conn);
	}

	int rowCount = result->row_count;	//get number of moves

	vector<Move> moveHistory;

	for (int i = 0; i < rowCount; i++) {
		row = mysql_fetch_row(result);
		Move t;
		t.move_pos = row[0];
		t.move_res = row[1];
		t.playerID = atoi(row[2]);

		ist.turns.push_back(t);
	}

	mysql_free_result(result);

	ss.clear();
	ss.str(string());
	return ist;
}




///serverside functions


int DBconnector::RegisterAI(string username)
{
	query = "INSERT INTO users(username,is_ai) VALUES('" + username + "',1)";
	if (PassQuery(query) != 0) {
		throw mysql_error(conn);
	}

	return Login(username);
}

int DBconnector::CreateAIlobby(string lobbyName, int AI_ID)
{
	int cLobby;
	try
	{
		cLobby = GetLobbyID(AI_ID);
	}
	catch (const char* e)
	{
		throw e;
	}

	if (cLobby != -1) {
		return cLobby;		//if a player is in a lobby return the lobbyID
	}

	ss << AI_ID;
	query = "INSERT INTO lobbies(lobby_name,adminID) VALUES('" + lobbyName + "'," + ss.str() + ")";
	ss.clear();
	ss.str(string());
	if (PassQuery(query) != 0) {
		throw mysql_error(conn);
	}

	query = "SELECT LAST_INSERT_ID()";	//get last inserted row index

	if (PassQuery(query) != 0) {
		throw mysql_error(conn);
	}

	MYSQL_RES* result = mysql_store_result(conn);

	if (result == NULL) {
		throw mysql_error(conn);
	}

	MYSQL_ROW row = mysql_fetch_row(result);

	cLobby = atoi(row[0]);
	mysql_free_result(result);

	return atoi(row[0]);
	return 0;
}

vector<int> DBconnector::GetReadyLobbies()
{
	vector<int> lobbyIDs;
	query = "SELECT lobbyID FROM lobbies WHERE lobby_status='r'";
    try {
	if (PassQuery(query) != 0) {
		throw mysql_error(conn);
	}
    }
    catch (int e) {
        cout << e << "\n";
    }
	MYSQL_RES* result = mysql_store_result(conn);

	if (result == NULL) {
		throw mysql_error(conn);
	}

	MYSQL_ROW row;
	int rowCount = result->row_count;	//get number of lobbies

	for (int i = 0; i < rowCount; i++) {
		row = mysql_fetch_row(result);
		lobbyIDs.push_back(atoi(row[0]));
	}

	mysql_free_result(result);
	return lobbyIDs;
}

int DBconnector::CreateHistoryTable(string game_name, int player1_ID, int player2_ID, string map1, string map2)
{
	int historyID;
	ss << player1_ID;
	string temp1 = ss.str();
	ss.clear();
	ss.str(string());
	ss << player2_ID;
	query = "INSERT INTO history(game_name,player1_ID,player2_ID, map1, map2) VALUES('"+game_name+"',"+temp1+","+ss.str()+",'"+map1+"','"+map2+"')";
	ss.clear();
	ss.str(string());
	if (PassQuery(query) != 0) {
		throw mysql_error(conn);
	}
	query = "SELECT LAST_INSERT_ID()";	//get last inserted row index

	if (PassQuery(query) != 0) {
		throw mysql_error(conn);
	}

	MYSQL_RES* result = mysql_store_result(conn);

	if (result == NULL) {
		throw mysql_error(conn);
	}

	MYSQL_ROW row = mysql_fetch_row(result);

	historyID = atoi(row[0]);
	mysql_free_result(result);
	return historyID;
}

void DBconnector::UpdateMoveTable(int gameID, int moveID, string move_pos, string move_res, int player_ID)  //gameid - historyID, moveID-from 0 to N, move_pos - userInput, move_res - consoleOutput, playerID- currentPlayer.
{
	ss << moveID;
	string temp1 = ss.str();
	ss.clear();
	ss.str(string());
	ss << player_ID;
	string temp2 = ss.str();
	ss.clear();
	ss.str(string());
	ss << gameID;
	string temp3 = ss.str();
	ss.clear();
	ss.str(string());

	query = "INSERT INTO moves (gameID, moveID, move_pos, move_res, playerID) VALUES("+temp3+","+temp1+",'"+move_pos+"','"+move_res+"',"+temp2+")";

	if (PassQuery(query) != 0) {
		throw mysql_error(conn);
	}
}

void DBconnector::UpdateLobby(int lobbyID, string lobby_status, string user_input, string console_output, string map1, string map2, int historyID, string game_status, int playerID)
{
	ss << historyID;
	string temp1 = ss.str();
	ss.clear();
	ss.str(string());
	ss << playerID;
	string temp2 = ss.str();
	ss.clear();
	ss.str(string());
	ss << lobbyID;
	string temp3 = ss.str();
	ss.clear();
	ss.str(string());
	query = "UPDATE lobbies SET lobby_status='"+lobby_status+"', user_input='"+user_input+"', console_output='"+console_output+"', admin_map='"+map1+"', opponent_map='"+map2+"', historyID="+temp1+",game_status='"+game_status+"', current_player="+temp2+" WHERE lobbyID="+temp3;

	if (PassQuery(query) != 0) {
		throw mysql_error(conn);
	}

}

DBconnector::ConsoleReadStruct DBconnector::ConsoleRead(int lobbyID)
{
	ConsoleReadStruct crs;
	ss << lobbyID;
	query = "SELECT adminID, opponentID, user_input, admin_map, opponent_map, game_status FROM lobbies WHERE lobbyID="+ss.str();
    
    if (PassQuery(query) != 0) {
        throw mysql_error(conn);
    }

	ss.clear();
	ss.str(string());
	MYSQL_RES* result = mysql_store_result(conn);

	if (result == NULL) {
		throw mysql_error(conn);
	}

	MYSQL_ROW row = mysql_fetch_row(result);
	crs.adminID = atoi(row[0]);
	crs.opponentID = atoi(row[1]);
	crs.user_input = row[2];
	crs.admin_map = row[3];
	crs.opponent_map = row[4];
	crs.game_status = row[5];
	mysql_free_result(result);

	return crs;
}

void DBconnector::InitiateDeletion(int lobbyID)
{
	ss << lobbyID;
	query = "DELETE FROM lobbies WHERE lobbyID="+ss.str();
	ss.clear();
	ss.str(string());
	if (PassQuery(query) != 0) {
		throw mysql_error(conn);
	}
}

void DBconnector::WriteWinner (int playerID, int gameID) {
    ss << playerID;
    string player = ss.str();
    ss.clear();
    ss.str(string());
    ss << gameID;
    query = "UPDATE history SET winnerID=" + player + " WHERE gameID=" + ss.str();
    ss.clear();
    ss.str(string());

    if (PassQuery(query) != 0) {
        throw mysql_error(conn);
    }
}
