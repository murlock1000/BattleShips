#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include "ClassesAndHeaders/Shot.h"
#include "ClassesAndHeaders/Laivas.h"
#include "ClassesAndHeaders/pch.h"
#include "ClassesAndHeaders/dbconnector.h"

using namespace std;

int userID;
int lobbyID;
DBconnector cnn;
vector<Laivas> Laivai[2];

int ReadFromFile(ifstream& file)
{
	int mbuffer;
	file >> mbuffer;
	return mbuffer;
}

sf::Vector2f GetGridPosition(int width, int height, int x, int y, sf::Vector2f size)
{
	sf::Vector2f buffer = sf::Vector2f((float)((x - 0.5375 * width) / size.x), (1.f + (float)((y - 0.075 * width)) / size.y)); ///turetu but kazkur height
	return (buffer.x > 0 && buffer.x < 10 && buffer.y > 0 && buffer.y < 10) ? buffer : sf::Vector2f(-1, -1);
}

bool AttempToConnect(string name, string password)
{
	return true;
}

void SetupShips(int lobbyID) {
	//Duomenys - Kintamieji
	bool AI[2] = { true, true }; //Ar 1/2 zaidejas - AI
	int ShipTable[10][10][2];
	//[y][x][player]

	pair<string,string> map = cnn.ReadMap(lobbyID);


		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				ShipTable[i][j][0] = map.first[i*10+j];
				ShipTable[i][j][1] = map.second[i * 10 + j];
			}
		}

	//Laivu kurimas
	for (int k = 0; k < 2; k++) {
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				if (ShipTable[i][j][k] != 0)
				{
					int xSize = 0;
					int ySize = 0;
					for (int l = 0; j + l < 10 && ShipTable[i][j + l][k] != 0; l++)
					{
						xSize++;
						ShipTable[i][j + l][k] = 0;
					}
					ShipTable[i][j][k] = 1;
					for (int l = 0; i + l < 10 && ShipTable[i + l][j][k] != 0; l++)
					{
						ySize++;
						ShipTable[i + l][j][k] = 0;
					}
					Laivai[k].push_back(Laivas(sf::Vector2f((float)j, (float)i), sf::Vector2f((float)(j + xSize - 1), (float)(i + ySize - 1))));

				}
			}
		}
	}

}

bool TryToMakeAShot(sf::Vector2f position, vector<Shot> &Shots, sf::RectangleShape gridas[10][10])
{
	bool isDuplicate = false;
	for (int i = 0; i < Shots.size(); i++)
	{
		if (Shots[i].gridPosition == position)
		{
			isDuplicate = true;
			return false;
		}
	}
	if (!isDuplicate)
	{
		Shots.push_back(Shot(position, gridas[int(position.x)][int(position.y)]));
		return true;
	}
	return false;
}


int main()
{

	try
	{
		cnn.Connect("localhost", "root", "root", "battleships");
	}
	catch (const char* e)
	{
		cout << e << endl;
		return 0;
	}

	//Pagrindinis langas
	unsigned int width = 1920 / 2, height = 1920 / 4;
	sf::RenderWindow window(sf::VideoMode(width, height), "Laivu musis", sf::Style::Resize | sf::Style::Close);
	window.setFramerateLimit(60);
	int Stage = 0;
	unsigned int NewWidth = width, NewHeight = height;

	//Fonas
	sf::RectangleShape background(sf::Vector2f((float)width, (float)height));
	sf::Texture areaTexture;
	areaTexture.loadFromFile("Media/Water.jpg");
	background.setTexture(&areaTexture);

	//Lobby fonas
	sf::RectangleShape LobbyBackground(sf::Vector2f((float)width, (float)height));
	sf::Texture LobbyTexture;
	LobbyTexture.loadFromFile("Media/Laivas_5.png");
	LobbyBackground.setTexture(&LobbyTexture);

	//Taisykles - Kintamieji
	const int TableWidth = 10;
	const int TableHeight = 10;
	const int LobbyWidth = 2;
	const int LobbyHeight = 10;
	const int PlayerCount = 2;
	const int LaivuCount = 5;


	//Suviu kurimas
	vector<Shot> Shots[PlayerCount];

	//Vaizdavimo Design'as ir parametrai
	//Pirmas kvadratas
	sf::RectangleShape Area1(sf::Vector2f((float)0.425 * width, (float)0.85 * height));
	Area1.setPosition(sf::Vector2f((float)0.0375 * width, (float)0.075 * height));
	Area1.setOutlineColor(sf::Color::Black);
	Area1.setOutlineThickness(2.f);
	Area1.setFillColor(sf::Color::Transparent);

	//Pirmo kvadrato sulangvavimas
	sf::RectangleShape Grid1[TableWidth][TableHeight];
	for (int i = 0; i < TableWidth; i++) {
		for (int j = 0; j < TableHeight; j++) {
			sf::Vector2f gridSize = sf::Vector2f(Area1.getSize().x * 0.1f, Area1.getSize().y * 0.1f);
			Grid1[i][j].setSize(gridSize);
			Grid1[i][j].setPosition(sf::Vector2f(Area1.getPosition().x + gridSize.x * i, Area1.getPosition().y + gridSize.y * j));
			Grid1[i][j].setOutlineColor(sf::Color::Black);
			Grid1[i][j].setOutlineThickness(1.f);
			Grid1[i][j].setFillColor(sf::Color::Transparent);
		}
	}


	//Antras kvadratas
	sf::RectangleShape Area2(sf::Vector2f((float)0.425 * width, (float)0.85 * height));
	Area2.setPosition(sf::Vector2f((float)0.5375 * width, (float)0.075 * height));
	Area2.setOutlineColor(sf::Color::Black);
	Area2.setOutlineThickness(2.f);
	Area2.setFillColor(sf::Color::Transparent);

	//Antro kvadrato sulangavimas
	sf::RectangleShape Grid2[TableWidth][TableHeight];
	for (int i = 0; i < TableWidth; i++) {
		for (int j = 0; j < TableHeight; j++) {
			sf::Vector2f gridSize = sf::Vector2f(Area2.getSize().x * 0.1f, Area2.getSize().y * 0.1f);
			Grid2[i][j].setSize(gridSize);
			Grid2[i][j].setPosition(sf::Vector2f(Area2.getPosition().x + gridSize.x * i, Area2.getPosition().y + gridSize.y * j));
			Grid2[i][j].setOutlineColor(sf::Color::Black);
			Grid2[i][j].setOutlineThickness(1.f);
			Grid2[i][j].setFillColor(sf::Color::Transparent);
		}
	}

	sf::RectangleShape LobbyArea(sf::Vector2f((float)0.85 * width, (float)0.85 * height));
	LobbyArea.setPosition(sf::Vector2f((float)0.075 * width, (float)0.075 * height));
	LobbyArea.setOutlineColor(sf::Color::Black);
	LobbyArea.setOutlineThickness(4.f);
	LobbyArea.setFillColor(sf::Color::Transparent);

	sf::RectangleShape LobbyGrid[LobbyWidth][LobbyHeight];
	for (int i = 0; i < LobbyWidth; i++) {
		for (int j = 0; j < LobbyHeight; j++) {
			sf::Vector2f LobbyGridSize = sf::Vector2f(LobbyArea.getSize().x * 0.5f, LobbyArea.getSize().y * 0.1f);
			LobbyGrid[i][j].setSize(LobbyGridSize);
			LobbyGrid[i][j].setPosition(sf::Vector2f(LobbyArea.getPosition().x + LobbyGridSize.x * i, LobbyArea.getPosition().y + LobbyGridSize.y * j));
			LobbyGrid[i][j].setOutlineColor(sf::Color::Black);
			LobbyGrid[i][j].setOutlineThickness(4.f);
			LobbyGrid[i][j].setFillColor(sf::Color::Transparent);
		}
	}
	/*
	//Laivu Grafiku kurimas
	for (int k = 0; k < PlayerCount; k++)
	{
		for (int i = 0; i < LaivuCount; i++)
		{
			(k == 0) ? Laivai[k][i].ConstructRectangle(Area1, Grid1[0][0].getSize()) : Laivai[k][i].ConstructRectangle(Area2, Grid2[0][0].getSize());
		}
	}
	sf::Texture Textures[4];
	for (int i = 2; i < 6; i++)
	{
		Textures[i - 2].loadFromFile("Media/Laivas_" + std::to_string(i) + ".png");
	}
	for (int k = 0; k < PlayerCount; k++)
	{
		for (int i = 0; i < LaivuCount; i++)
		{
			if (Laivai[k][i].IsRectangleRotated())
			{
				Laivai[k][i].SetRectangleTexture(Textures[Laivai[k][i].GetLaivasLength() - 2]);
				Laivai[k][i].RotateRectangle(90.f);
			}
			else
			{
				Laivai[k][i].SetRectangleTexture(Textures[Laivai[k][i].GetLaivasLength() - 2]);
			}
		}
	}
	*/

	//font load
	//sf::Font Comicsas;
	//Comicsas.loadFromFile("Media/comic.ttf");
	//sf::Text login;
	//login.setFont(Comicsas);
	//login.setString("login");
	//login.setCharacterSize(24);
	//login.setFillColor(sf::Color::Black);

	sf::Texture Button_Textures[2];
	for (int i = 0; i < 2; i++)
	{
		Button_Textures[i].loadFromFile("Media/Button_" + std::to_string(i) + ".png");
	}

	string Zem = "0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 0, 4, 0, 5, 0, 0, 0, 0, 0, 0, 0, 4, 0, 5, 0, 0, 0, 0, 2, 2, 0, 4, 0, 5, 0, 0, 0, 0, 0, 0, 0, 4, 0, 5, 0, 0, 0, 1, 1, 1, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ";

	sf::RectangleShape login_button(sf::Vector2f(200, 40));
	login_button.setPosition(sf::Vector2f(380, 140));
	login_button.setOutlineColor(sf::Color::Black);
	login_button.setOutlineThickness(2.f);
	login_button.setFillColor(sf::Color::Blue);
	login_button.setTexture(&Button_Textures[0]);

	sf::RectangleShape spectate_button(sf::Vector2f(0.2f * width, 0.07f * height));
	spectate_button.setPosition(sf::Vector2f(0.4f * width, 0.5f * height));
	spectate_button.setOutlineColor(sf::Color::Black);
	spectate_button.setOutlineThickness(2.f);
	spectate_button.setFillColor(sf::Color::Cyan);
	spectate_button.setTexture(&Button_Textures[1]);

	DBconnector::Rlobby rlobby;
	//rlobby = cnn.ReadLobby(lobbyID);
	string enemyMove;
	string DidYouMakeIt; //ar pataikei?
	string userInput = "";
	vector<DBconnector::LobbyTable> lobbies;

	int langas = 1;
	while (window.isOpen())
	{
		sf::Event event;
		switch (langas)
		{
		case 0: //zaidimas
			//Eventu Handlinimas
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
				if (event.type == sf::Event::MouseButtonPressed)
				{
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						int x = event.mouseButton.x;
						int y = event.mouseButton.y;
						sf::Vector2f gridPosition = GetGridPosition(NewWidth, NewHeight, x, y, sf::Vector2f(Area2.getSize().x * 0.1f * ((float)NewWidth / (float)width), Area2.getSize().y * 0.1f * ((float)NewHeight / (float)height)));
						if (gridPosition.x != -1 && gridPosition.y != -1)
						{
							//Šuvis
							if (rlobby.curr_player == userID & rlobby.game_status == "w") {
								if (TryToMakeAShot(gridPosition, Shots[0], Grid2))
								{
									userInput = std::to_string(gridPosition.x) + "-" + std::to_string(gridPosition.y);
									if (userInput != "") {
										cnn.WriteMove(lobbyID, userInput);
									}
								}
							}
						}
					}
					/*if (event.mouseButton.button == sf::Mouse::Right)
					{
						langas = 1;
					}*/
				}
				if (event.type == sf::Event::Resized)
				{
					NewWidth = event.size.width;
					NewHeight = event.size.height;
				}
			}

		
			if (rlobby.curr_player == userID & rlobby.game_status == "e") {
				enemyMove = rlobby.console_output; 
				//display enemy move
				cout << enemyMove; // mes negalim atvaizduoti pataike ar nepataike...
				//TryToMakeAShot(sf::Vector2f(0, 0 /*x koordinate, y koordinate*/), Shots[0], Grid1);
				//inform user and wait for his action
				cout << "jusu eile" << endl;
				cnn.WriteMove(lobbyID, "ok");
			}


			if (rlobby.curr_player != userID) {
				DidYouMakeIt = rlobby.console_output;
			}

			//Piesimas
			window.clear();
			window.draw(background);
			window.draw(Area1);
			window.draw(Area2);
			for (int i = 0; i < TableWidth; i++) {
				for (int j = 0; j < TableHeight; j++) {
					window.draw(Grid1[i][j]);
					window.draw(Grid2[i][j]);
				}
			}
			for (int k = 0; k < PlayerCount; k++)
			{
				for (int i = 0; i < LaivuCount; i++)
				{
					window.draw(Laivai[k][i].GetLaivasRectangle());
				}
			}
			for (int k = 0; k < PlayerCount; k++)
			{
				for (int i = 0; i < Shots[k].size(); i++)
				{
					window.draw(Shots[k][i].GetShotRect());
				}

			}
			window.display();
			break;
		case 1: //login
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
				if (event.type == sf::Event::Resized)
				{
					NewWidth = event.size.width;
					NewHeight = event.size.height;
				}
				if (event.type == sf::Event::MouseButtonPressed)
				{
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						int x = event.mouseButton.x / ((float)NewWidth / (float)width);
						int y = event.mouseButton.y / ((float)NewHeight / (float)height);
						if (x > 380 && x < 580)
						{
							if (y > 140 && y < 180) //login cia //SURAST KAIP PADARYT TEXT BOX
							{
								string Name = "Zaidejas1"; //cia kintamieji, kur veliau padarysim, kad galetum irasyti
								string Password = "Skupas123";
								
								try
								{
									userID = cnn.Login(Name);
									langas = 2; //imest funkcija, kuri chekintu login'a, ir tada grazintu i langas 2 (lobby)
								}
								catch (const char* e)
								{
									cout << e << endl;
									langas=1;
								}
							}
							else if (y > 240 && y < 280) //register mygtukas
							{
								//kai sukursim tada sita sugalvosim
							}
						}
					}
					if (event.mouseButton.button == sf::Mouse::Right)
					{
						langas = 1;
					}
				}
			}
			window.clear();
			window.draw(background);
			window.draw(login_button);
			window.draw(spectate_button);
			window.display();
			break;
		case 2: //lobby
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
				if (event.type == sf::Event::Resized)
				{
					NewWidth = event.size.width;
					NewHeight = event.size.height;
				}
			}
			window.clear();
			window.draw(background);
			window.draw(LobbyArea);

			try
			{
				lobbies = cnn.ListLobbies(1);
			}
			catch (const char* e)
			{
				cout << e << endl;
				return 0;
			}
		
			
			for (int i = 0; i < LobbyWidth; i++) {
				for (int j = 0; j < lobbies.size(); j++) {
					window.draw(LobbyGrid[i][j]);
					window.draw(LobbyGrid[i][j]);
				}
			}
			window.display();
			break;

		case 3: //loading screen
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
				if (event.type == sf::Event::Resized)
				{
					NewWidth = event.size.width;
					NewHeight = event.size.height;
				}
			}
			window.clear();
			window.draw(background);
			cout << "loaaadinggg"<<endl;
			window.display();
			
			cnn.JoinLobbyAsPlayer(lobbyID, userID);

				while (true) {

						rlobby = cnn.ReadLobby(lobbyID);

					if (rlobby.console_output == "w" && rlobby.curr_player == userID) {
						cnn.WriteMove(lobbyID, "100char map");
						break;
					}

				while (true) {
					if (rlobby.console_output == "w" && rlobby.curr_player == userID) {
						SetupShips(lobbyID);
						break;
					}
				}
				langas = 0;
			}

		default:
			return 0;
			break;
		}
	}
	return 0;
}
