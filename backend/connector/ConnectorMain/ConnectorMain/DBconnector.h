#pragma once
#include <mysql.h>
#include <string>
class DBconnector
{
private:
	int N;
	MYSQL *conn;
public:
	DBconnector();
	~DBconnector();
	int ReadN();
	void Connect(std::string,std::string,std::string,std::string);
};

