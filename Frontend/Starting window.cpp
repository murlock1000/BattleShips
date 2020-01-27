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
#include <fstream>

using namespace std;

int userID;
int lobbyID;
DBconnector cnn;
vector<Laivas> Laivai;

struct ScreenSizeProperties
{
	unsigned int width;
	unsigned int height;
	unsigned int NewWidth;
	unsigned int NewHeight;
};

int ReadFromFile(ifstream& file)
{
	int mbuffer;
	file >> mbuffer;
	return mbuffer;
}

sf::Vector2i GetGridPosition(ScreenSizeProperties& sc, int x, int y, sf::Vector2f size)
{
	//cout << "pressed xy: " << x << " " << y <<" "<<size.x<<" "<<size.y<< endl;
	//sf::Vector2i buffer = sf::Vector2i((float)((x - 0.5375 * sc.width) / size.x), (1.f + (float)((y - 0.075 * sc.width)) / size.y)); ///turetu but kazkur sc.height cia savo laukui nuspaust
	sf::Vector2i buffer = sf::Vector2i((float)((x - 0.0375* sc.width) / size.x), (1.f + (float)((y - 0.075 * sc.width)) / size.y)); ///turetu but kazkur sc.height enemy field 1.
	//cout << "Buffer: " << buffer.x << " " << buffer.y << endl;
	return (buffer.x >= 0 && buffer.x < 10 && buffer.y >= 0 && buffer.y < 10) ? buffer : sf::Vector2i(-1, -1);
}

bool AttempToConnect(string name, string password)
{
	return true;
}

//void SetupShipsOld(int lobbyID) {
//	//Duomenys - Kintamieji
//	bool AI[2] = { true, true }; //Ar 1/2 zaidejas - AI
//	int ShipTable[10][10][2];
//	//[y][x][player]
//
//	pair<string, string> map = cnn.ReadMap(lobbyID);
//
//
//	for (int i = 0; i < 10; i++)
//	{
//		for (int j = 0; j < 10; j++)
//		{
//			ShipTable[i][j][0] = map.first[i * 10 + j];
//			ShipTable[i][j][1] = map.second[i * 10 + j];
//		}
//	}
//
//	//Laivu kurimas
//	for (int k = 0; k < 2; k++) {
//		for (int i = 0; i < 10; i++) {
//			for (int j = 0; j < 10; j++) {
//				if (ShipTable[i][j][k] != 0)
//				{
//					int xSize = 0;
//					int ySize = 0;
//					for (int l = 0; j + l < 10 && ShipTable[i][j + l][k] != 0; l++)
//					{
//						xSize++;
//						ShipTable[i][j + l][k] = 0;
//					}
//					ShipTable[i][j][k] = 1;
//					for (int l = 0; i + l < 10 && ShipTable[i + l][j][k] != 0; l++)
//					{
//						ySize++;
//						ShipTable[i + l][j][k] = 0;
//					}
//					Laivai.push_back(Laivas(sf::Vector2f((float)j, (float)i), sf::Vector2f((float)(j + xSize - 1), (float)(i + ySize - 1))));
//
//				}
//			}
//		}
//	}
//
//}

void SetupShips(string PlayerMap, int TableWidth, int TableHeight) {
	//Duomenys - Kintamieji

	cout << "h " << TableHeight << " - " << TableWidth << endl;

	vector<vector<int>> ShipTable(TableHeight);
	//[y][x]

	ShipTable.resize(TableHeight);
	cout << PlayerMap << endl;
	for (int i = 0; i < TableHeight; i++)
	{
		ShipTable[i].resize(TableWidth);
		for (int j = 0; j < TableWidth; j++)
		{

			
			ShipTable[i][j] = PlayerMap[i * 10 + j] - '0';
			cout << ShipTable[i][j] << " ";
		}
		cout << endl;
	}

	//Laivu kurimas

	for (int i = 0; i < TableHeight; i++) {
		for (int j = 0; j < TableWidth; j++) {
			if (ShipTable[i][j] != 0)
			{
				int xSize = 0;
				int ySize = 0;
				for (int l = 0; j + l < 10 && ShipTable[i][j + l] != 0; l++)
				{
					xSize++;
					ShipTable[i][j + l] = 0;
				}
				ShipTable[i][j] = 1;
				for (int l = 0; i + l < 10 && ShipTable[i + l][j] != 0; l++)
				{
					ySize++;
					ShipTable[i + l][j] = 0;
				}
				Laivai.push_back(Laivas(sf::Vector2f((float)j, (float)i), sf::Vector2f((float)(j + xSize - 1), (float)(i + ySize - 1))));

			}
		}
	}

}

bool TryToMakeAShot(sf::Vector2i position, vector<Shot>& Shots, vector<vector<sf::RectangleShape>> gridas)
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

void SetupLoginScreen(vector<sf::RectangleShape>& loginGraphics, ScreenSizeProperties sc, sf::Texture Button_Textures[2], sf::Texture& areaTexture) {
	//Fonas

	sf::RectangleShape background(sf::Vector2f((float)sc.width, (float)sc.height));
	background.setTexture(&areaTexture);

	//mygtuko tekstūros

	//string Zem = "0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 0, 4, 0, 5, 0, 0, 0, 0, 0, 0, 0, 4, 0, 5, 0, 0, 0, 0, 2, 2, 0, 4, 0, 5, 0, 0, 0, 0, 0, 0, 0, 4, 0, 5, 0, 0, 0, 1, 1, 1, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ";

	//login mygtukas
	sf::RectangleShape login_button(sf::Vector2f(200, 40));
	login_button.setPosition(sf::Vector2f(380, 140));
	login_button.setOutlineColor(sf::Color::Black);
	login_button.setOutlineThickness(2.f);
	login_button.setFillColor(sf::Color::Blue);
	login_button.setTexture(&Button_Textures[0]);

	//spectate mygtukas
	sf::RectangleShape spectate_button(sf::Vector2f(0.2f * sc.width, 0.07f * sc.height));
	spectate_button.setPosition(sf::Vector2f(0.4f * sc.width, 0.5f * sc.height));
	spectate_button.setOutlineColor(sf::Color::Black);
	spectate_button.setOutlineThickness(2.f);
	spectate_button.setFillColor(sf::Color::Cyan);
	spectate_button.setTexture(&Button_Textures[1]);

	loginGraphics.push_back(background);
	loginGraphics.push_back(login_button);
	loginGraphics.push_back(spectate_button);
	//lobbyGraphics[0] = &background;
//	lobbyGraphics[1] = &login_button;
	//lobbyGraphics[2] = &spectate_button;
}

void LoginScreen(sf::RenderWindow& window, sf::Event& event, ScreenSizeProperties& sc, int& langas, vector<sf::RectangleShape>& graphics) {
	//sf::RectangleShape a = *graphics[0];

	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
		if (event.type == sf::Event::Resized)
		{
			sc.NewWidth = event.size.width;
			sc.NewHeight = event.size.height;
		}
		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				int x = event.mouseButton.x / ((float)sc.NewWidth / (float)sc.width);
				int y = event.mouseButton.y / ((float)sc.NewHeight / (float)sc.height);
				if (x > 380 && x < 580)
				{
					if (y > 140 && y < 180) //login cia //SURAST KAIP PADARYT TEXT BOX
					{
						string Name = "test1"; //cia kintamieji, kur veliau padarysim, kad galetum irasyti
						string Password = "Skupas123";

						try
						{
							userID = cnn.Login(Name);
							cout << "userID: " << userID << endl;
							langas = 2; //imest funkcija, kuri chekintu login'a, ir tada grazintu i langas 2 (lobby)

						}
						catch (const char* e)
						{
							cout << e << endl;
							langas = 1;
						}
					}
					else if (y > 240 && y < 280) //register mygtukas
					{
						//kai sukursim tada sita sugalvosim
					}
				}
			}
			if (event.mouseButton.button == sf::Mouse::Right) //šitas nieko nedaro xD
			{
				langas = 1;
			}
		}
	}

	window.clear();
	window.draw(graphics[0]);//background
	window.draw(graphics[1]);//login_button
	window.draw(graphics[2]);//spectate_button
	window.display();
}

void SetupLobbyListScreen(vector<sf::RectangleShape>& lobbyGraphics, ScreenSizeProperties& sc, const int LobbyWidth, const int LobbyHeight, sf::Texture& areaTexture) {

	//background texture 
	sf::RectangleShape background(sf::Vector2f((float)sc.width, (float)sc.height));
	background.setTexture(&areaTexture);
	lobbyGraphics.push_back(background);

	//lobby area
	sf::RectangleShape LobbyArea(sf::Vector2f((float)0.85 * sc.width, (float)0.85 * sc.height));
	LobbyArea.setPosition(sf::Vector2f((float)0.075 * sc.width, (float)0.075 * sc.height));
	LobbyArea.setOutlineColor(sf::Color::Black);
	LobbyArea.setOutlineThickness(4.f);
	LobbyArea.setFillColor(sf::Color::Transparent);

	lobbyGraphics.push_back(LobbyArea);


	//lobby sulangavimas
	/*
	LobbyGrid = new sf::Text * [LobbyWidth];

	for (int i = 0; i < LobbyWidth; i++) {
		LobbyGrid[i] = new sf::Text[LobbyHeight];
	}
	//[LobbyWidth] [LobbyHeight] ;
	for (int i = 0; i < LobbyWidth; i++) {
		for (int j = 0; j < LobbyHeight; j++) {
			sf::Vector2f LobbyGridSize = sf::Vector2f(LobbyArea.getSize().x * 0.5f, LobbyArea.getSize().y * 0.1f);
			//LobbyGrid[i][j].setSize(LobbyGridSize);
			LobbyGrid[i][j].setFont(Comicsas);
			//LobbyGrid[i][j].setString("login");
			LobbyGrid[i][j].setCharacterSize(24);
			LobbyGrid[i][j].setPosition(sf::Vector2f(LobbyArea.getPosition().x + LobbyGridSize.x * i, LobbyArea.getPosition().y + LobbyGridSize.y * j));
			//LobbyGrid[i][j].setOutlineColor(sf::Color::Black);
			//LobbyGrid[i][j].setOutlineThickness(4.f);
			LobbyGrid[i][j].setFillColor(sf::Color::Black);
		}
	}*/
}

void DisplayLobbies(vector<DBconnector::LobbyTable> lobbies, sf::RenderWindow& window, vector<sf::RectangleShape> graphics, vector<sf::Text>(&LobbyGrid)[2], sf::Font& Comicsas) {

	sf::Vector2f LobbyGridSize = sf::Vector2f(graphics[1].getSize().x * 0.5f, graphics[1].getSize().y * 0.1f);


	sf::Text gameNameDesc("Game name", Comicsas, 24);
	sf::Text adminNameDesc("Admin name", Comicsas, 24);

	gameNameDesc.setPosition(sf::Vector2f(graphics[1].getPosition().x, graphics[1].getPosition().y));
	adminNameDesc.setPosition(sf::Vector2f(graphics[1].getPosition().x + LobbyGridSize.x, graphics[1].getPosition().y));

	LobbyGrid[0].push_back(gameNameDesc);
	LobbyGrid[1].push_back(adminNameDesc);


	for (int i = 0; i < lobbies.size(); i++) {

		sf::Text gameName;
		sf::Text adminName;

		gameName.setFont(Comicsas);
		adminName.setFont(Comicsas);

		gameName.setCharacterSize(24);
		adminName.setCharacterSize(24);

		gameName.setPosition(sf::Vector2f(graphics[1].getPosition().x, graphics[1].getPosition().y + LobbyGridSize.y * (i + 1)));
		adminName.setPosition(sf::Vector2f(graphics[1].getPosition().x + LobbyGridSize.x, graphics[1].getPosition().y + LobbyGridSize.y * (i + 1)));

		gameName.setFillColor(sf::Color::Black);
		adminName.setFillColor(sf::Color::Black);

		gameName.setString(lobbies[i].lobbyName);
		adminName.setString(lobbies[i].adminName);

		LobbyGrid[0].push_back(gameName);
		LobbyGrid[1].push_back(adminName);
	}

}

void LobbyListScreen(sf::RenderWindow& window, sf::Event& event, ScreenSizeProperties& sc, int& langas, vector<sf::RectangleShape> graphics) {
	int x, y;
	sf::RectangleShape pele;
	vector<DBconnector::LobbyTable> lobbies;

	sf::Font Comicsas;	//loading font
	if (!Comicsas.loadFromFile("Media/arial.ttf")) cout << "error";

	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
		if (event.type == sf::Event::Resized)
		{
			sc.NewWidth = event.size.width;
			sc.NewHeight = event.size.height;
		}
		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				x = event.mouseButton.x / ((float)sc.NewWidth / (float)sc.width);
				y = event.mouseButton.y / ((float)sc.NewHeight / (float)sc.height);
				pele.setSize(sf::Vector2f(1.f, 1.f));
				pele.setPosition(sf::Vector2f(x, y));
			}
		}
	}
	window.clear();
	window.draw(graphics[0]);//background);
	window.draw(graphics[1]);//LobbyArea);

	lobbies = cnn.ListLobbies(1);
	vector<sf::Text> LobbyGrid[2]; //a cell for game name and opponent name each

	DisplayLobbies(lobbies, window, graphics, LobbyGrid, Comicsas);

	//cout << "size of lobby " << LobbyGrid[0].size() << endl;

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < LobbyGrid[0].size(); j++) {
			//cout << "i and j: " << i << " " << j << endl;
			if (LobbyGrid[i][j].getGlobalBounds().intersects(pele.getGlobalBounds()))
			{
				if (j != 0) {
					lobbyID = lobbies[j - 1].lobbyID;
					cout << "Selected lobbyID: " << lobbyID << endl;
					try
					{
						cnn.JoinLobbyAsPlayer(lobbyID, userID);
					}
					catch (const char* s)
					{
						cout << "error: " << s << endl;
					}
					cout << "langas: " << langas << endl;
					langas++; //for the memes
					cout << "langasN: " << langas << endl;
				}
			}
			window.draw(LobbyGrid[i][j]);
		}
	}


	window.display();
}

void SetupGameScreen(sf::RenderWindow& window, ScreenSizeProperties& sc, int TableWidth, int TableHeight, vector<vector<sf::RectangleShape>> &Grid1, vector<vector<sf::RectangleShape>> &Grid2, int LaivuCount, sf::Texture Textures[4], sf::RectangleShape &Area1, sf::RectangleShape &Area2) {

	//Vaizdavimo Design'as ir parametrai
	//Pirmas kvadratas

	Area1.setSize(sf::Vector2f((float)0.425 * sc.width, (float)0.85 * sc.height));// = new sf::RectangleShape(sf::Vector2f((float)0.425 * sc.width, (float)0.85 * sc.height));
	Area1.setPosition(sf::Vector2f((float)0.0375 * sc.width, (float)0.075 * sc.height));
	Area1.setOutlineColor(sf::Color::Black);
	Area1.setOutlineThickness(2.f);
	Area1.setFillColor(sf::Color::Transparent);

	//Pirmo kvadrato sulangvavimas
	Grid1.resize(TableWidth);
	for (int i = 0; i < TableWidth; i++) {
		Grid1[i].resize(TableHeight);
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
	Area2.setSize(sf::Vector2f((float)0.425 * sc.width, (float)0.85 * sc.height));
	Area2.setPosition(sf::Vector2f((float)0.5375 * sc.width, (float)0.075 * sc.height));
	Area2.setOutlineColor(sf::Color::Black);
	Area2.setOutlineThickness(2.f);
	Area2.setFillColor(sf::Color::Transparent);

	//Antro kvadrato sulangavimas
	Grid2.resize(TableWidth);
	for (int i = 0; i < TableWidth; i++) {
		Grid2[i].resize(TableHeight);
		for (int j = 0; j < TableHeight; j++) {
			sf::Vector2f gridSize = sf::Vector2f(Area2.getSize().x * 0.1f, Area2.getSize().y * 0.1f);
			Grid2[i][j].setSize(gridSize);
			Grid2[i][j].setPosition(sf::Vector2f(Area2.getPosition().x + gridSize.x * i, Area2.getPosition().y + gridSize.y * j));
			Grid2[i][j].setOutlineColor(sf::Color::Black);
			Grid2[i][j].setOutlineThickness(1.f);
			Grid2[i][j].setFillColor(sf::Color::Transparent);
		}
	}
	//zaidejo laivu vaizdavimas
	for (int i = 0; i < LaivuCount; i++)
	{
		Laivai[i].ConstructRectangle(Area2, Grid2[0][0].getSize());
	}

	for (int i = 0; i < LaivuCount; i++)
	{
		if (Laivai[i].IsRectangleRotated())
		{
			Laivai[i].SetRectangleTexture(Textures[Laivai[i].GetLaivasLength() - 2]);
			Laivai[i].RotateRectangle(90.f);
		}
		else
		{
			Laivai[i].SetRectangleTexture(Textures[Laivai[i].GetLaivasLength() - 2]);
		}
	}

}

//void DisplayShipsOLD(sf::RenderWindow& window, sf::Event& event, ScreenSizeProperties& sc, int& langas, vector<sf::RectangleShape> graphics) {
//	//Suviu kurimas
//	vector<Shot> Shots[PlayerCount];
//
//
//	for (int k = 0; k < PlayerCount; k++)
//	{
//		for (int i = 0; i < LaivuCount; i++)
//		{
//			(k == 0) ? Laivai[k][i].ConstructRectangle(Area1, Grid1[0][0].getSize()) : Laivai[k][i].ConstructRectangle(Area2, Grid2[0][0].getSize());
//		}
//	}
//	sf::Texture Textures[4];
//	for (int i = 2; i < 6; i++)
//	{
//		Textures[i - 2].loadFromFile("Media/Laivas_" + std::to_string(i) + ".png");
//	}
//	for (int k = 0; k < PlayerCount; k++)
//	{
//		for (int i = 0; i < LaivuCount; i++)
//		{
//			if (Laivai[k][i].IsRectangleRotated())
//			{
//				Laivai[k][i].SetRectangleTexture(Textures[Laivai[k][i].GetLaivasLength() - 2]);
//				Laivai[k][i].RotateRectangle(90.f);
//			}
//			else
//			{
//				Laivai[k][i].SetRectangleTexture(Textures[Laivai[k][i].GetLaivasLength() - 2]);
//			}
//		}
//	}
//
//
//	/*	//Laivu Grafiku kurimas
//		for (int k = 0; k < PlayerCount; k++)
//		{
//			for (int i = 0; i < LaivuCount; i++)
//			{
//				(k == 0) ? Laivai[k][i].ConstructRectangle(Area1, Grid1[0][0].getSize()) : Laivai[k][i].ConstructRectangle(Area2, Grid2[0][0].getSize());
//			}
//		}
//		sf::Texture Textures[4];
//		for (int i = 2; i < 6; i++)
//		{
//			Textures[i - 2].loadFromFile("Media/Laivas_" + std::to_string(i) + ".png");
//		}
//		for (int k = 0; k < PlayerCount; k++)
//		{
//			for (int i = 0; i < LaivuCount; i++)
//			{
//				if (Laivai[k][i].IsRectangleRotated())
//				{
//					Laivai[k][i].SetRectangleTexture(Textures[Laivai[k][i].GetLaivasLength() - 2]);
//					Laivai[k][i].RotateRectangle(90.f);
//				}
//				else
//				{
//					Laivai[k][i].SetRectangleTexture(Textures[Laivai[k][i].GetLaivasLength() - 2]);
//				}
//			}
//		}*/
//}

int main()
{
	try
	{
		cnn.Connect("localhost", "root", "root", "battleships");
	}
	catch (const std::exception&)
	{
		cout << "Failed to connect to database" << endl;
		return 0;
	}

	//Pagrindinis langas
	ScreenSizeProperties sc;
	sc.width = 1920 / 2;
	sc.height = 1920 / 4;

	sf::RenderWindow window(sf::VideoMode(sc.width, sc.height), "Laivu musis", sf::Style::Resize | sf::Style::Close);
	window.setFramerateLimit(60);
	int Stage = 0;
	sc.NewWidth = sc.width;
	sc.NewHeight = sc.height;

	/*//Lobby fonas
	sf::RectangleShape LobbyBackground(sf::Vector2f((float)sc.width, (float)sc.height));
	sf::Texture LobbyTexture;
	LobbyTexture.loadFromFile("Media/Laivas_5.png");
	LobbyBackground.setTexture(&LobbyTexture);
	*/
	//Taisykles - Kintamieji
	const int TableWidth = 10;
	const int TableHeight = 10;
	const int LobbyWidth = 2;
	const int LobbyHeight = 10;
	const int PlayerCount = 2;
	const int LaivuCount = 5;

	//font load
	//sf::Text login;
	//login.setFont(Comicsas);
	//login.setString("login");
	//login.setCharacterSize(24);
	//login.setFillColor(sf::Color::Black);

	sf::Texture areaTexture;	//must be kept at all times because objects don't keep a copy, but a pointer to it
	areaTexture.loadFromFile("Media/Water.jpg");

	sf::RectangleShape background(sf::Vector2f((float)sc.width, (float)sc.height));
	background.setTexture(&areaTexture);


	//Login screen graphics setup'
	sf::Texture Button_Textures[2];//same as areaTexture
	for (int i = 0; i < 2; i++)
	{
		Button_Textures[i].loadFromFile("Media/Button_" + std::to_string(i) + ".png");
	}
	vector<sf::RectangleShape> LoginGraphics;//= { background,login_button,spectate_button };
	SetupLoginScreen(LoginGraphics, sc, Button_Textures, areaTexture);



	//Lobby list graphics setup
	vector<sf::RectangleShape> LobbyGraphics;//= { background,LobbyArea };
	SetupLobbyListScreen(LobbyGraphics, sc, LobbyWidth, LobbyHeight, areaTexture);
	DBconnector::Rlobby rlobby;



	//loading  screen
	sf::Font Comicsas;	//loading font
	if (!Comicsas.loadFromFile("Media/arial.ttf")) cout << "error";
	sf::Text pInput;
	pInput.setFont(Comicsas);
	pInput.setString("Write your map name");
	pInput.setCharacterSize(24);
	pInput.setFillColor(sf::Color::Black);
	pInput.setPosition(sf::Vector2f(250, 140));
	string mapPath = "./.txt";

	string PlayerMap = "";
	//string PlayerMap = "0011111000000000000000033304440000000000000020000000002000000000200000000020000000000000000000000055";
	int timeout = 3000;

	///	rlobby = cnn.ReadLobby(lobbyID);


	//Game graphics setup

	sf::Texture Textures[4];
	for (int i = 2; i < 6; i++)
	{
		Textures[i - 2].loadFromFile("Media/Laivas_" + std::to_string(i) + ".png");
	}

	sf::RectangleShape Area1;
	sf::RectangleShape Area2;
	vector<vector<sf::RectangleShape>> Grid1;
	vector<vector<sf::RectangleShape>> Grid2;
	//Suviu kurimas
	vector<Shot> Shots[PlayerCount];//0 - zaidejas 1- priesininkas


	string enemyMove;
	string DidYouMakeIt; //ar pataikei?
	string userInput = "";

	int langas = 1;
	int waitingFor = 0;

	//SetupShips(PlayerMap, TableWidth, TableHeight); // gauname vektoriu laivu su ju koordinatemis
	//SetupGameScreen(window, sc, TableWidth, TableHeight, Grid1, Grid2, LaivuCount, Textures,Area1,Area2); //pradedame zaidimo lauko vaizdavima

	while (window.isOpen())
	{
		sf::Event event;
		switch (langas)
		{
		case 0:

			switch (waitingFor)
			{
			case 0: //waiting for your turn
				while (window.pollEvent(event))
				{
					if (event.type == sf::Event::Closed)
						window.close();

					if (event.type == sf::Event::Resized)
					{
						sc.NewWidth = event.size.width;
						sc.NewHeight = event.size.height;
					}
				}
				try
				{
					rlobby = cnn.ReadLobby(lobbyID);
				}
				catch (const char* s)
				{
					cout << "FAIL: " << s << endl;
				}

				if (rlobby.curr_player == userID & rlobby.game_status == "w") { //sulauke musu ejimo nuskaitome priesininko ejima
					enemyMove = rlobby.console_output;
					//display enemy move
					int x, y;
					int dashPosition = 0;

					cout << "EnemyMove: " << enemyMove << endl;
					if (enemyMove.size() > 2) {
						while (enemyMove.substr(dashPosition, 1) != "-") dashPosition++;
						try
						{
							x = stoi(enemyMove.substr(0, dashPosition));
							y = stoi(enemyMove.substr(dashPosition + 1, enemyMove.length() - 1));
							TryToMakeAShot(sf::Vector2i(x, y), Shots[1], Grid2); //display enemy move
						}
						catch (const std::exception&)
						{
							cout << "server returned invalid message or someone won and the end game function has not been implemented yet" << endl;
							//return -1;
						}
					}


					cout << enemyMove; // mes negalim atvaizduoti pataike ar nepataike...
					//TryToMakeAShot(sf::Vector2f(0, 0 //x koordinate, y koordinate), Shots[0], Grid1);
					//inform user and wait for his action
					cout << "jusu eile" << endl;
					//cnn.WriteMove(lobbyID, "ok");

				


					waitingFor = 1;
				}
				break;
			case 1:				//waiting for player's move selection
				while (window.pollEvent(event))
				{
					if (event.type == sf::Event::Closed)
						window.close();

					if (event.type == sf::Event::Resized)
					{
						sc.NewWidth = event.size.width;
						sc.NewHeight = event.size.height;
					}

					if (event.type == sf::Event::MouseButtonPressed)
					{
						if (event.mouseButton.button == sf::Mouse::Left)
						{
							int x = event.mouseButton.x;
							int y = event.mouseButton.y;
							sf::Vector2i gridPosition = GetGridPosition(sc, x, y, sf::Vector2f(Area1.getSize().x * 0.1f * ((float)sc.NewWidth / (float)sc.width), Area1.getSize().y * 0.1f * ((float)sc.NewHeight / (float)sc.height)));//STEADY AND PRECISE AIM
							if (gridPosition.x != -1 && gridPosition.y != -1)
							{
								//Šuvis GO!
								if (TryToMakeAShot(gridPosition, Shots[0], Grid1))
								{
									userInput = std::to_string((int)gridPosition.y) + "-" + std::to_string((int)gridPosition.x);
									if (userInput != "") {
										cnn.WriteMove(lobbyID, userInput);
										waitingFor = 2;
									}
								}

							}
						}
					}
				}
				break;
			case 2: //waiting for console's response to the move
				while (window.pollEvent(event))
				{
					if (event.type == sf::Event::Closed)
						window.close();

					if (event.type == sf::Event::Resized)
					{
						sc.NewWidth = event.size.width;
						sc.NewHeight = event.size.height;
					}
				}
				try
				{
					rlobby = cnn.ReadLobby(lobbyID);
				}
				catch (const char* s)
				{
					cout << "FAIL: " << s << endl;
				}

				if (rlobby.curr_player == userID & rlobby.game_status == "w") {
					DidYouMakeIt = rlobby.console_output;
					//display result of your action
					cout << "rezultatas: " << DidYouMakeIt << endl; // mes negalim atvaizduoti pataike ar nepataike... but we can :)
					waitingFor = 0;
				}
				break;

			default:
				break;
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

			for (int i = 0; i < LaivuCount; i++)//atvaizduojame zaidejo laivus
			{
				window.draw(Laivai[i].GetLaivasRectangle());
			}

			for (int k = 0; k < PlayerCount; k++)	//atvaizduojame abieju zaideju suvius 
			{
				for (int i = 0; i < Shots[k].size(); i++)
				{
					window.draw(Shots[k][i].GetShotRect());
				}

			}
			window.display();
			break;
		case 1: //login screen

			LoginScreen(window, event, sc, langas, LoginGraphics);

			break;
		/*case 2:
			//Piesimas

			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();

				if (event.type == sf::Event::Resized)
				{
					sc.NewWidth = event.size.width;
					sc.NewHeight = event.size.height;
					//Area1.setSize(sf::Vector2f((float)0.425 * sc.NewWidth, (float)0.85 * sc.NewHeight));
				//	Area2.setSize(sf::Vector2f((float)0.425 * sc.NewWidth, (float)0.85 * sc.NewHeight));
				}

				if (event.type == sf::Event::MouseButtonPressed)
				{
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						int x = event.mouseButton.x;
						int y = event.mouseButton.y;
						//cout << "Area " << Area2.getSize().x << endl;
						sf::Vector2i gridPosition = GetGridPosition(sc, x, y, sf::Vector2f(Area1.getSize().x * 0.1f * ((float)sc.NewWidth / (float)sc.width), Area1.getSize().y * 0.1f * ((float)sc.NewHeight / (float)sc.height)));//STEADY AND PRECISE AIM
					//	cout << "GridPos: " << gridPosition.x << "-" << gridPosition.y << endl;
						if (gridPosition.x != -1 && gridPosition.y != -1)
						{
							//Šuvis GO!
							if (TryToMakeAShot(gridPosition, Shots[0], Grid1))
							{
								userInput = std::to_string((int)gridPosition.y) + "-" + std::to_string((int)gridPosition.x);
								cout <<"aimPos: "<< userInput << endl;
							}

						}
					}
				}
			}


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

			for (int i = 0; i < LaivuCount; i++)//atvaizduojame zaidejo laivus
			{
				//cout << Laivai[i].ilgis << endl;
				window.draw(Laivai[i].GetLaivasRectangle());
			}

			for (int k = 0; k < PlayerCount; k++)	//atvaizduojame abieju zaideju suvius 
			{
				for (int i = 0; i < Shots[k].size(); i++)
				{
					window.draw(Shots[k][i].GetShotRect());
				}

			}
			window.display();
			break;
			*/
		case 2: //lobby list //2

			LobbyListScreen(window, event, sc, langas, LobbyGraphics);
			break;

		case 3: //loading screen 3 game.exe ijungiama, irasome failo pavadinima su 100 skaiciu, reprezentuojanciu musu laivu isdestyma, laukiama patvirtinimo ir pradedamas zaidimas

		//	lobbyID = 19;

			try
			{
				rlobby = cnn.ReadLobby(lobbyID);
			}
			catch (const char* s)
			{
				cout << "FAIL: " << s << endl;
			}

		//	cout << rlobby.console_output << "-" << rlobby.curr_player << "-" << rlobby.game_status << endl;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
				if (event.type == sf::Event::Resized)
				{
					sc.NewWidth = event.size.width;
					sc.NewHeight = event.size.height;
				}
				if (rlobby.game_status == "w" && rlobby.curr_player == userID) { //jeigu game.exe bendrauja su mumis
					pInput.setString(mapPath);
					if (event.type == sf::Event::TextEntered)
					{
						if (event.text.unicode == 8) { //jeigu backspace - istriname
							if (mapPath.size() > 6) {
								mapPath = mapPath.substr(0, mapPath.size() - 5);
								mapPath += ".txt";
								pInput.setString(mapPath);
							}
						}
						else if (event.text.unicode == 13) { //enter submittina faila
							ifstream fin(mapPath);
							char t;
							cout << "trying" << endl;
							for (int i = 0; i < 100; i++) { //perduodame 100 numeriu po viena, laukdami patvirtinimo pries issiunciant nauja skaiciu
								fin >> t;
								string ts;
								ts = t;
								PlayerMap += ts;
								try
								{
									cnn.WriteMove(lobbyID, ts);
								}
								catch (const char* s)
								{
									//	cout << endl;
										//cout << "size: " << PlayerMap.size() << endl;
										//cout << PlayerMap << endl;
									cout << "error when submitting map: " << s << endl;
									return -1;
								}
								cout << ts << " ";

								timeout = 1000; // laukiame max 10s serveriui patvirtinti musu input
								do {
									try
									{
										rlobby = cnn.ReadLobby(lobbyID);
									}
									catch (const char* s)
									{
										cout << "FAIL: " << s << endl;
									}
									window.clear();
									window.draw(background);
									pInput.setString("Loading...");
									window.draw(pInput);
									window.display();

									Sleep(10);
									timeout--;
								} while (rlobby.game_status != "w" && timeout > 0);//wait for console to read output

								if (timeout == 0) { //server crashed
									return -1;
								}
							}
							cout << endl;
							langas = 0; // nukreipiame vaizda i zaidima
							cout << "INGAME" << endl;
							cout << endl;
							cout << PlayerMap << endl;
							SetupShips(PlayerMap, TableWidth, TableHeight); // gauname vektoriu laivu su ju koordinatemis
							SetupGameScreen(window, sc, TableWidth, TableHeight, Grid1, Grid2, LaivuCount, Textures,Area1,Area2); //pradedame zaidimo lauko vaizdavima
						}
						else {
							mapPath = mapPath.substr(0, mapPath.size() - 4);
							mapPath += event.text.unicode;
							mapPath += ".txt";
							pInput.setString(mapPath);
						}
					}
				}
				else {
					pInput.setString("Waiting for others...");
				}

			}
			window.clear();
			window.draw(background);
			window.draw(pInput);
			window.display();
			break;
		default:
			cout << "WHY " << langas << endl;
			return 0;
			break;
		}
	}
	return 0;
}
