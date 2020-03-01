//This utility is meant to be a simple way to register AIs

#include <iostream>
#include "dbconnector.h"

using namespace std;

int registerAI (char* aiName) {
	DBconnector dbc;
	if (dbc.Connect ("127.0.0.1", "ServerAdmin", "admin", "battleships") < 0) { //connect to the database
		cout << "error: (register-ai) Unable to connect to database\n";
		return 1;
	}
	stringstream aiName_s;
	aiName_s << aiName;
	dbc.RegisterAI(aiName_s.str()); //register the AI
	return 0;
}
