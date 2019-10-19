#pragma once
#include <mysql.h>
#include <string>
#include <sstream>
#include <cppconn/statement.h>
#include <vector>
using namespace std;

class DBconnector
{
	
private:
	MYSQL *conn; 
	string query;
	stringstream ss;
	int PassQuery(string q) {
		return mysql_query(conn, q.c_str());
	}
	void SetUserLobby(int userID, int lobbyID);
//	sql::Statement *stmt;
//	sql::PreparedStatement *pstmt;
public:

	struct LobbyTable {
		int lobbyID;
		string lobbyName;
		string adminName;
	};

	DBconnector();
	~DBconnector();
	void Connect(string ip,string username,string password,string dbName);
	int Register(string username);
	int Login(string username);
	void ChangeName(string newUsername, int userID);
	pair<int,int> GetInfo(int userID);
	int GetLobbyID(int userID);
	int CreateLobby(string lobbyName, int userID);
	void LeaveLobby(int lobbyID, int userID);
	vector<LobbyTable> ListLobbiesAsPlayer(boolean isUser);
	int JoinLobbyAsSpectator(int lobbyID);
	MYSQL_RES ReadLobby(int lobbyID);
	pair<string,string> ReadMap(int lobbyID);
	void WriteMove(int, string, int);
	void Leave(int);
	void AcknowledgeLoss(int, int);
	string GetWinner(int);
	MYSQL_RES List();
	MYSQL_RES GetInfoOnGame(int);
};

