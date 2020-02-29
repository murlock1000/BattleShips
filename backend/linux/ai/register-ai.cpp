//This utility is meant to be a simple way to register AIs

#include <iostream>
#include "../dbconnector.h"

using namespace std;

int main (int argc, char* argv []) {
    if (argc == 1) { //if there are no arguments provided:
        cerr << "USAGE: battleships-register-ai AI_NAME_1 [AI_NAME_2] ...\n";
        return 0;
    }
    else { //we have at least one argument provided
        DBconnector dbc;
        if (dbc.Connect ("127.0.0.1", "ServerAdmin", "admin", "battleships") < 0) { //connect to the database
			cout << "ERROR: Unable to connect to database\n";
			return 1;
		}
        for (int i = 1; i < argc; i ++) { //one can provide more than one AI, we need to register them all
            stringstream aiName;
            aiName << argv [i];
            dbc.RegisterAI(aiName.str()); //register the AI
            aiName.clear();
            aiName.str(string());
        }
        return 0;
    }
}