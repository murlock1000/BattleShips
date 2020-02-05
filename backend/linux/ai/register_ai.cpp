//This utility is meant to be a simple way to register AIs

#include <iostream>
#include "../dbconnector/dbconnector.h"

using namespace std;

int main (int argc, char* argv []) {
    if (argc == 1) { //if there are no arguments provided:
        cerr << "USAGE: register_ai ai_name_1 [ai_name_2] ...\n";
        return 1;
    }
    else { //we have at least one argument provided
        DBconnector dbc;
        dbc.Connect ("127.0.0.1", "root", "password", "battleships"); //connect to the database
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
