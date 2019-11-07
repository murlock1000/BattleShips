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

	struct UserInfoTable {
		string username;
		int wins;
		int losses;
	};

	struct LobbyTable {
		int lobbyID;
		string lobbyName;
		string adminName;
		string opponentName;
	};

	struct Rlobby {
		string game_status;
		string console_output;
		int curr_player;
	};

	struct Move {
		int playerID;
		string move_pos;
		string move_res;
	};

	struct History {
		string game_name;
		int player1_ID;
		int player2_ID;
		string map1;
		string map2;
		int winnerID;
		vector<Move> turns;	

	};

	DBconnector();
	~DBconnector();
	void Connect(string ip,string username,string password,string dbName);
	int Register(string username);
	int Login(string username);
	void ChangeName(string newUsername, int userID);
	UserInfoTable GetUserInfo(int userID);
	int GetLobbyID(int userID);
	int CreateLobby(string lobbyName, int userID);
	void LeaveLobby(int lobbyID, int userID);
	vector<LobbyTable> ListLobbies(boolean isUser);
	void JoinLobbyAsPlayer(int lobbyID, int userID);
	Rlobby ReadLobby(int lobbyID);
	pair<string,string> ReadMap(int lobbyID);
	void WriteMove(int lobbyID, string userIn);
	int AcknowledgeEnd(int lobbyID, int userID, bool isUser);
	string GetWinner(int);	//not recommended to use when getting all information about the game, replace with GetInfoOnGame and GetUserInfo
	History GetInfoOnGame(int gameID);
};
