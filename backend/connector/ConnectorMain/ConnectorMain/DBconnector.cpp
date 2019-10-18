#include "pch.h"
#include "DBconnector.h"
#include <mysql.h>
#include <iostream>
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

