// ConnectorMain.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <mysql.h>
#include "DBconnector.h"

using namespace std;

int main()
{
	DBconnector cnn;
	int user1;
	int user2;
	int lobbyID=3;
	try
	{
		cnn.Connect("localhost", "root", "password", "battleships");
	}
	catch (const char* e)
	{
		cout << e << endl;
	}



	try
	{
		user1 = cnn.Login("Matas");
	}
	catch (const char* e)
	{
		cout << e << endl;
	}

	try
	{
		user2 = cnn.Login("Paulius");
	}
	catch (const char* e)
	{
		cout << e << endl;
	}

/*	try
	{
		lobbyID = cnn.CreateLobby("Mato Zaidimas", user1);
	}
	catch (const char* e)
	{
		cout << e << endl;
	}

	*/
	/*try
	{
		//cout << lobbyID << " A " << user2 << endl;
		cnn.JoinLobbyAsPlayer(lobbyID, user2);
	}
	catch (const char* e)
	{
		cout <<"cia "<< e << endl;
	}
	*/
	vector<DBconnector::LobbyTable> tables = cnn.ListLobbies(1);

	for (int i = 0; i < tables.size(); i++) {
		cout << tables[i].lobbyID << " " << tables[i].lobbyName << " " << tables[i].adminName << " " << tables[i].opponentName << endl;
	}

	DBconnector::Rlobby lobby;

	
	try
	{
		lobby = cnn.ReadLobby(lobbyID);
	}
	catch (const char* e)
	{
		cout << e << endl;
	}

	cout << "lobby3: " << lobby.game_status << " " << lobby.curr_player << " " << lobby.console_output << endl;

	




		return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
