#include "pch.h"
#include "DBconnector.h"
#include <mysql.h>
#include <iostream>
#include <sstream>
#include <cppconn/statement.h>
using namespace std;

DBconnector::DBconnector()
{
	this->N = 5;

}

int DBconnector::ReadN() {
	return this->N;
}

DBconnector::~DBconnector()
{
}

void DBconnector::Connect(string ip,string username,string pass,string database)
{

	conn = mysql_init(0);

	conn = mysql_real_connect(conn, ip.c_str(), username.c_str(), pass.c_str(), database.c_str(), 3306, NULL, 0);
	if (conn)
	{
		cout << "works" << endl;
	}
	else {
		cout << "not" << endl;
	}
}

void DBconnector::Register(std::string username)
{
	query = "INSERT INTO users(username,is_ai) VALUES(" + username + ",0)";
}

void DBconnector::Login(std::string username)
{
	query = "SELECT userID FROM users WHERE username="+username;
}

void DBconnector::ChangeName(std::string newName, int userID)
{
	ss << userID;
	query = "UPDATE users SET username="+newName+" WHERE userID="+ss.str();
	ss.clear();
}

MYSQL_RES DBconnector::GetInfo(int userID)
{
	ss << userID;
	query = "SELECT wins,losses FROM users WHERE userID="+ss.str();
	ss.clear();
	return MYSQL_RES();
}

void DBconnector::CreateLobby(std::string lobbyName, std::string opDescription, int userID)
{
	ss << userID;
	query = "INSERT INTO lobbies(lobby_name,description,adminID) VALUES("+lobbyName+","+opDescription+","+ss.str());
	ss.clear();
}

MYSQL_RES DBconnector::ListLobbiesAsPlayer(boolean isUser)
{
	if (isUser) {
		query = "SELECT lobbyID, lobby_name, description FROM lobbies WHERE lobby_status=‘w‘";
	}
	else query = "SELECT lobbyID, lobby_name, description FROM lobbies WHERE lobby_status=‘w‘";
	return MYSQL_RES();
}

MYSQL_RES DBconnector::JoinLobbyAsSpectator(int)
{
	query = "";
	return MYSQL_RES();
}

MYSQL_RES DBconnector::ReadLobby(int)
{
	query = "";
	return MYSQL_RES();
}

MYSQL_RES DBconnector::ReadMap(int)
{
	query = "";
	return MYSQL_RES();
}

void DBconnector::WriteMove(int, std::string, int)
{
	query = "";
}

void DBconnector::Leave(int)
{
	query = "";
}

void DBconnector::AcknowledgeLoss(int, int)
{
	query = "";
}

MYSQL_RES DBconnector::GetWinner(int)
{
	query = "";
	return MYSQL_RES();
}

MYSQL_RES DBconnector::List()
{
	query = "";
	return MYSQL_RES();
}

MYSQL_RES DBconnector::GetInfoOnGame(int)
{
	query = "";
	return MYSQL_RES();
}







