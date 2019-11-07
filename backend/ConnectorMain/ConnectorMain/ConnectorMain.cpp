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
	int userID;
	int lobbyID;
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
		userID=cnn.Login("Matas");
	}
	catch (const char* e)
	{
		cout << e << endl;
	}
	cout << userID << endl;
	
	/*try
	{
		lobbyID = cnn.CreateLobby("Test", userID);
	}
	catch (const char* e)
	{
		cout <<"EXCEPTION: "<< e << endl;
	}
	cout << "lobbyID: " << lobbyID << endl;
	*/
/*	try
	{
		cnn.LeaveLobby(lobbyID, userID);
	}
	catch (const char* e)
	{
		cout << "EXCEPTION: " << e << endl;
	}
	cout << "left" << endl;*/
	vector<DBconnector::LobbyTable> tbl;

/*	try
	{
		tbl = cnn.ListLobbiesAsPlayer(1);
	}
	catch (const char* e)
	{
		cout << "EXCEPTION: " << e << endl;
	}*/

	for (int i = 0; i < tbl.size(); i++) {
		cout << tbl[i].lobbyID << " " << tbl[i].lobbyName << " " << tbl[i].adminName << endl;
	}
	
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
