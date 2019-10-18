#pragma once
#include <mysql.h>
#include <string>
#include <sstream>
#include <cppconn/statement.h>
class DBconnector
{
private:
	int N;
	MYSQL *conn; 
	std::string query;
	stringstream ss;
//	sql::Statement *stmt;
//	sql::PreparedStatement *pstmt;
public:
	DBconnector();
	~DBconnector();
	int ReadN();
	void Connect(std::string,std::string,std::string,std::string);
	void Register(std::string);
	void Login(std::string);
	void ChangeName(std::string, int);
	MYSQL_RES GetInfo(int);
	void CreateLobby(std::string, std::string, int);
	MYSQL_RES ListLobbiesAsPlayer(boolean);
	MYSQL_RES JoinLobbyAsSpectator(int);
	MYSQL_RES ReadLobby(int);
	MYSQL_RES ReadMap(int);
	void WriteMove(int, std::string, int);
	void Leave(int);
	void AcknowledgeLoss(int, int);
	MYSQL_RES GetWinner(int);
	MYSQL_RES List();
	MYSQL_RES GetInfoOnGame(int);
};

