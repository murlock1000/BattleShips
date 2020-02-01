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
string username = "type the username";
int lobbyID;
DBconnector cnn;
vector<Laivas> Laivai;

struct ScreenSizeProperties
{
	unsigned int width;
	unsigned int height;
	unsigned int NewWidth;
	unsigned int NewHeight;
}sc;

//Taisykles - Kintamieji
const int TableWidth = 10;
const int TableHeight = 10;
const int LobbyWidth = 2;
const int LobbyHeight = 10;
const int LaivuCount = 5;


int ReadFromFile(ifstream& file)
{
	int mbuffer;
	file >> mbuffer;
	return mbuffer;
}

sf::Vector2i GetGridPosition(int x, int y, sf::Vector2f size)
{
	//cout << "pressed xy: " << x << " " << y <<" "<<size.x<<" "<<size.y<< endl;
	//sf::Vector2i buffer = sf::Vector2i((float)((x - 0.5375 * sc.width) / size.x), (1.f + (float)((y - 0.075 * sc.width)) / size.y)); ///turetu but kazkur sc.height cia savo laukui nuspaust
	sf::Vector2i buffer = sf::Vector2i((float)((x - 0.0375 * sc.width) / size.x), (1.f + (float)((y - 0.075 * sc.width)) / size.y)); ///turetu but kazkur sc.height enemy field 1.
	//cout << "Buffer: " << buffer.x << " " << buffer.y << endl;
	return (buffer.x >= 0 && buffer.x < 10 && buffer.y >= 0 && buffer.y < 10) ? buffer : sf::Vector2i(-1, -1);
}

bool AttempToConnect(string name, string password)
{
	return true;
}

void SetupShips(string PlayerMap) {
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

void ClearCatche(int& historyId, vector<Shot>(&Shots)[2], vector<vector<sf::RectangleShape>>(&Grid)[2], int& waitingFor) {
	
	for (int i = 0; i < 2;i++) {
		Shots[i].clear();
	}

	waitingFor = 0;
	historyId = 0;
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

void SetupApplicationGraphics(map<string, sf::RectangleShape>& graphics, map<string, sf::Texture>& textures, map<string, sf::Text>& texts, map<string, sf::Font>& fonts) {
	
#pragma region General_design

	//Fonas
	sf::RectangleShape background(sf::Vector2f((float)sc.width, (float)sc.height));
	background.setTexture(&textures["water"]);
	graphics["background"] = background;

	//back mygtukas
	sf::RectangleShape back_button(sf::Vector2f(100, 40));
	back_button.setPosition(sf::Vector2f(780, 400));
	back_button.setOutlineColor(sf::Color::Black);
	back_button.setOutlineThickness(2.f);
	back_button.setFillColor(sf::Color::Blue);
	back_button.setTexture(&textures["back"]);
	graphics["back"] = back_button;

#pragma endregion General_design

#pragma region Login_design
	///Login screen buttons

	//login mygtukas
	sf::RectangleShape login_button(sf::Vector2f(200, 40));
	login_button.setPosition(sf::Vector2f(380, 140));
	login_button.setOutlineColor(sf::Color::Black);
	login_button.setOutlineThickness(2.f);
	login_button.setFillColor(sf::Color::Blue);
	login_button.setTexture(&textures["login"]);
	graphics["login_btn"] = login_button;

	//spectate mygtukas
	sf::RectangleShape spectate_button(sf::Vector2f(0.2f * sc.width, 0.1f * sc.height));
	spectate_button.setPosition(sf::Vector2f(0.4f * sc.width, 0.48f * sc.height));
	spectate_button.setOutlineColor(sf::Color::Black);
	spectate_button.setOutlineThickness(2.f);
	spectate_button.setFillColor(sf::Color::Cyan);
	spectate_button.setTexture(&textures["spectate"]);
	graphics["spectate_btn"] = spectate_button;

	//register mygtukas
	sf::RectangleShape register_button(sf::Vector2f(0.2f * sc.width, 0.1f * sc.height));
	register_button.setPosition(sf::Vector2f(0.4f * sc.width, 0.6f * sc.height));
	register_button.setOutlineColor(sf::Color::Black);
	register_button.setOutlineThickness(2.f);
	register_button.setFillColor(sf::Color::Cyan);
	register_button.setTexture(&textures["register"]);
	graphics["register_btn"] = register_button;


	//username text box
	sf::Text user_txt;
	user_txt.setFont(fonts["arial"]);
	user_txt.setString("username");
	user_txt.setCharacterSize(26);
	user_txt.setFillColor(sf::Color::Black);
	user_txt.setPosition(sf::Vector2f(200, 100));
	texts["username"] = user_txt;

#pragma endregion Login_design

#pragma region Lobby_design  
	///Lobby screen buttons, border

	//lobby area
	sf::RectangleShape LobbyArea(sf::Vector2f((float)0.70 * sc.width, (float)0.85 * sc.height));
	LobbyArea.setPosition(sf::Vector2f((float)0.075 * sc.width, (float)0.075 * sc.height));
	LobbyArea.setOutlineColor(sf::Color::Black);
	LobbyArea.setOutlineThickness(4.f);
	LobbyArea.setFillColor(sf::Color::Transparent);
	graphics["lobby_area"] = LobbyArea;

	//Create Create lobby button
	sf::RectangleShape CreateLobby_button(sf::Vector2f(180, 50));
	CreateLobby_button.setPosition(sf::Vector2f((float)0.8 * sc.width, (float)0.075 * sc.height));
	CreateLobby_button.setOutlineColor(sf::Color::Black);
	CreateLobby_button.setOutlineThickness(2.f);
	CreateLobby_button.setFillColor(sf::Color::Blue);
	CreateLobby_button.setTexture(&textures["createLobby"]);
	graphics["createLobby_btn"] = CreateLobby_button;

#pragma endregion Lobby_design

#pragma region Game_design

	//Pirmas kvadratas
	sf::RectangleShape Area1(sf::Vector2f((float)0.425 * sc.width, (float)0.85 * sc.height));
	Area1.setPosition(sf::Vector2f((float)0.0375 * sc.width, (float)0.075 * sc.height));
	Area1.setOutlineColor(sf::Color::Black);
	Area1.setOutlineThickness(2.f);
	Area1.setFillColor(sf::Color::Transparent);
	graphics["area1"] = Area1;

	//Antras kvadratas
	sf::RectangleShape Area2(sf::Vector2f((float)0.425 * sc.width, (float)0.85 * sc.height));
	Area2.setPosition(sf::Vector2f((float)0.5375 * sc.width, (float)0.075 * sc.height));
	Area2.setOutlineColor(sf::Color::Black);
	Area2.setOutlineThickness(2.f);
	Area2.setFillColor(sf::Color::Transparent);
	graphics["area2"] = Area2;

	//map file path

	sf::Text mapPath;
	mapPath.setFont(fonts["arial"]);
	mapPath.setString("Write your map name");
	mapPath.setCharacterSize(24);
	mapPath.setFillColor(sf::Color::Black);
	mapPath.setPosition(sf::Vector2f(250, 140));
	texts["mapPath"] = mapPath;

#pragma endregion Game_design  

#pragma region Lobby_creation
	//lobby name textbox

	sf::Text lobbyName;
	lobbyName.setFont(fonts["arial"]);
	lobbyName.setString("Lobby name");
	lobbyName.setCharacterSize(24);
	lobbyName.setFillColor(sf::Color::Black);
	lobbyName.setPosition(sf::Vector2f(250, 140));
	texts["lobbyName"] = lobbyName;

#pragma endregion Lobby_creation
}

void LoginScreen(sf::RenderWindow& window, sf::Event& event, int& langas, map<string, sf::RectangleShape>& graphics, map<string, sf::Text>& texts) {
	//sf::RectangleShape a = *graphics[0];
	int x, y;
	sf::RectangleShape pele;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
		if (event.type == sf::Event::Resized)
		{
			sc.NewWidth = event.size.width;
			sc.NewHeight = event.size.height;
		}

		texts["username"].setString(username);
		if (event.type == sf::Event::TextEntered)
		{
			if (event.text.unicode == 8) {
				if (username.size() > 0) {
					username = username.substr(0, username.size() - 1);
					texts["username"].setString(username);
				}
			}
			else {
				username += event.text.unicode;
				texts["username"].setString(username);
			}
		}

		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				x = event.mouseButton.x / ((float)sc.NewWidth / (float)sc.width);
				y = event.mouseButton.y / ((float)sc.NewHeight / (float)sc.height);
				pele.setSize(sf::Vector2f(1.f, 1.f));
				pele.setPosition(sf::Vector2f(x, y));

				//	sf::FloatRect a = graphics["login_btn"].getGlobalBounds();

				if ((graphics["login_btn"].getGlobalBounds()).intersects(pele.getGlobalBounds())) {
					try
					{
						userID = cnn.Login(username);
						cout << "userID: " << userID << endl;
						langas = 2; //imest funkcija, kuri chekintu login'a, ir tada grazintu i langas 2 (lobby)

					}
					catch (const char* e)
					{
						cout << e << endl;
						langas = 1;
					}
				}
				else if (graphics["spectate_btn"].getGlobalBounds().intersects(pele.getGlobalBounds())) {
					cout << "Move along stranger" << endl;
				}
				else if (graphics["register_btn"].getGlobalBounds().intersects(pele.getGlobalBounds())) {
					try
					{
						userID = cnn.Register(username);
						langas = 2;
					}
					catch (const char* e)
					{
						cout << e << endl;

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
	window.draw(graphics["background"]);//background
	window.draw(graphics["login_btn"]);//login_button
	window.draw(graphics["spectate_btn"]);//spectate_button
	window.draw(graphics["register_btn"]);//register
	window.draw(texts["username"]);
	window.display();
}

void DisplayLobbies(vector<DBconnector::LobbyTable> &lobbies, sf::RenderWindow& window, map<string, sf::RectangleShape>& graphics, vector<sf::Text>(&LobbyGrid)[2], map<string, sf::Text>& texts, map<string, sf::Font> &fonts) {

	sf::Vector2f LobbyGridSize = sf::Vector2f(graphics["lobby_area"].getSize().x * 0.5f, graphics["lobby_area"].getSize().y * 0.1f);


	sf::Text gameNameDesc("Game name", fonts["arial"], 24);
	sf::Text adminNameDesc("Admin name", fonts["arial"], 24);

	gameNameDesc.setPosition(sf::Vector2f(graphics["lobby_area"].getPosition().x, graphics["lobby_area"].getPosition().y));
	adminNameDesc.setPosition(sf::Vector2f(graphics["lobby_area"].getPosition().x + LobbyGridSize.x, graphics["lobby_area"].getPosition().y));

	LobbyGrid[0].push_back(gameNameDesc);
	LobbyGrid[1].push_back(adminNameDesc);


	for (int i = 0; i < lobbies.size(); i++) {

		sf::Text gameName;
		sf::Text adminName;

		gameName.setFont(fonts["arial"]);
		adminName.setFont(fonts["arial"]);

		gameName.setCharacterSize(24);
		adminName.setCharacterSize(24);

		gameName.setPosition(sf::Vector2f(graphics["lobby_area"].getPosition().x, graphics["lobby_area"].getPosition().y + LobbyGridSize.y * (i + 1)));
		adminName.setPosition(sf::Vector2f(graphics["lobby_area"].getPosition().x + LobbyGridSize.x, graphics["lobby_area"].getPosition().y + LobbyGridSize.y * (i + 1)));

		gameName.setFillColor(sf::Color::Black);
		adminName.setFillColor(sf::Color::Black);

		gameName.setString(lobbies[i].lobbyName);
		adminName.setString(lobbies[i].adminName);

		LobbyGrid[0].push_back(gameName);
		LobbyGrid[1].push_back(adminName);
	}

}

void LobbyListScreen(sf::RenderWindow& window, sf::Event& event, int& langas, map<string, sf::RectangleShape>& graphics, map<string, sf::Font> &fonts, map<string, sf::Text>& texts) {
	int x, y;
	sf::RectangleShape pele;
	vector<DBconnector::LobbyTable> lobbies;

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

			if (graphics["createLobby_btn"].getGlobalBounds().intersects(pele.getGlobalBounds())) {

				langas = 4;
			}

			if (graphics["back"].getGlobalBounds().intersects(pele.getGlobalBounds())) {

				langas = 1;
			}
		}
	}
	window.clear();
	window.draw(graphics["background"]);//background);
	window.draw(graphics["lobby_area"]);//LobbyArea);
	window.draw(graphics["createLobby_btn"]);//CreateLobby);
	window.draw(graphics["back"]);//back);
	
	lobbies = cnn.ListLobbies(1);
	vector<sf::Text> LobbyGrid[2]; //a cell for game name and opponent name each
	
	DisplayLobbies(lobbies, window, graphics, LobbyGrid,texts,fonts);
	
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

void SetupGameScreen(sf::RenderWindow& window, vector<vector<sf::RectangleShape>>(&Grid)[2], map<string, sf::RectangleShape>& graphics, map<string, sf::Texture>& textures) {

	//Vaizdavimo Design'as ir parametrai

	//Pirmo kvadrato sulangvavimas
	Grid[0].resize(TableWidth);
	for (int i = 0; i < TableWidth; i++) {
		Grid[0][i].resize(TableHeight);
		for (int j = 0; j < TableHeight; j++) {
			sf::Vector2f gridSize = sf::Vector2f(graphics["area1"].getSize().x * 0.1f, graphics["area1"].getSize().y * 0.1f);
			Grid[0][i][j].setSize(gridSize);
			Grid[0][i][j].setPosition(sf::Vector2f(graphics["area1"].getPosition().x + gridSize.x * i, graphics["area1"].getPosition().y + gridSize.y * j));
			Grid[0][i][j].setOutlineColor(sf::Color::Black);
			Grid[0][i][j].setOutlineThickness(1.f);
			Grid[0][i][j].setFillColor(sf::Color::Transparent);
		}
	}

	//Antro kvadrato sulangavimas
	Grid[1].resize(TableWidth);
	for (int i = 0; i < TableWidth; i++) {
		Grid[1][i].resize(TableHeight);
		for (int j = 0; j < TableHeight; j++) {
			sf::Vector2f gridSize = sf::Vector2f(graphics["area2"].getSize().x * 0.1f, graphics["area2"].getSize().y * 0.1f);
			Grid[1][i][j].setSize(gridSize);
			Grid[1][i][j].setPosition(sf::Vector2f(graphics["area2"].getPosition().x + gridSize.x * i, graphics["area2"].getPosition().y + gridSize.y * j));
			Grid[1][i][j].setOutlineColor(sf::Color::Black);
			Grid[1][i][j].setOutlineThickness(1.f);
			Grid[1][i][j].setFillColor(sf::Color::Transparent);
		}
	}
	//zaidejo laivu vaizdavimas
	for (int i = 0; i < LaivuCount; i++)
	{
		Laivai[i].ConstructRectangle(graphics["area2"], Grid[1][0][0].getSize());
	}

	for (int i = 0; i < LaivuCount; i++)
	{
		if (Laivai[i].IsRectangleRotated())
		{
			Laivai[i].SetRectangleTexture(textures["Laivas_" + to_string(Laivai[i].GetLaivasLength())]);
			Laivai[i].RotateRectangle(90.f);
		}
		else
		{
			Laivai[i].SetRectangleTexture(textures["Laivas_" + to_string(Laivai[i].GetLaivasLength())]);
		}
	}

}

void LoadingScreen(sf::RenderWindow& window, sf::Event& event, map<string, sf::RectangleShape>& graphics, map<string, sf::Text>& texts, int& langas,string &mapPath, string& PlayerMap, vector<vector<sf::RectangleShape>>(&Grid)[2], map<string, sf::Texture>& textures) {
	//	lobbyID = 19;
	int timeout;

	DBconnector::Rlobby rlobby;
	try
	{
		rlobby = cnn.ReadLobby(lobbyID);
	}
	catch (const char* s)
	{
		cout << "FAIL: " << s << endl;
	}

	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed) {
			try
			{
				cnn.LeaveLobby(lobbyID, userID);
			}
			catch (const char* s)
			{
				cout << s << endl;
			}

			window.close();
		}
		if (event.type == sf::Event::Resized)
		{
			sc.NewWidth = event.size.width;
			sc.NewHeight = event.size.height;
		}
		if (rlobby.game_status == "w" && rlobby.curr_player == userID) { //jeigu game.exe bendrauja su mumis
			texts["mapPath"].setString(mapPath);
			if (event.type == sf::Event::TextEntered)
			{
				if (event.text.unicode == 8) { //jeigu backspace - istriname
					if (mapPath.size() > 6) {
						mapPath = mapPath.substr(0, mapPath.size() - 5);
						mapPath += ".txt";
						texts["mapPath"].setString(mapPath);
					}
				}
				else if (event.text.unicode == 13) { //enter submittina faila
					ifstream fin(mapPath);
					char t;
					cout << "trying" << endl;
					PlayerMap = "";//isvalome buvusi map
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
							langas = 2;//going back to lobby list
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
							window.draw(graphics["background"]);
							texts["mapPath"].setString("Loading...");
							window.draw(texts["mapPath"]);
							window.display();

							Sleep(10);
							timeout--;
						} while (rlobby.game_status != "w" && timeout > 0);//wait for console to read output

						if (timeout == 0) { //server crashed
							cout << "server crashed (question mark)" << endl;
							langas = 2;//going back to lobby list
						}
					}
					cout << endl;
					langas = 0; // nukreipiame vaizda i zaidima
					cout << "INGAME" << endl;
					cout << endl;
					cout << PlayerMap << endl;
					SetupShips(PlayerMap); // gauname vektoriu laivu su ju koordinatemis
					SetupGameScreen(window, Grid, graphics, textures);
				}
				else {
					mapPath = mapPath.substr(0, mapPath.size() - 4);
					mapPath += event.text.unicode;
					mapPath += ".txt";
					texts["mapPath"].setString(mapPath);
				}
			}
		}
		else {
			texts["mapPath"].setString("Waiting for others...");
		}

	}
	window.clear();
	window.draw(graphics["background"]);
	window.draw(texts["mapPath"]);
	window.display();
}

void GameScreen(sf::RenderWindow& window, sf::Event& event, map<string, sf::RectangleShape>& graphics, map<string, sf::Text>& texts, int& langas,int& historyId, vector<Shot>(&Shots)[2], vector<vector<sf::RectangleShape>> (&Grid)[2], map<string, sf::Texture>& textures, int &waitingFor) {

	
	//variables for debbuging
	string enemyMove;
	string DidYouMakeIt; //ar pataikei?
	string userInput;
	cout << "waiting for: " << waitingFor << endl;

	DBconnector::Rlobby rlobby;
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

			//	cout << "EnemyMove: " << enemyMove << endl;
			if (enemyMove.size() > 2) {
				while (enemyMove.substr(dashPosition, 1) != "-") dashPosition++;
				try
				{
					x = stoi(enemyMove.substr(0, dashPosition));
					y = stoi(enemyMove.substr(dashPosition + 1, enemyMove.length() - 1));
					TryToMakeAShot(sf::Vector2i(x, y), Shots[1], Grid[1]); //display enemy move
				}
				catch (const std::exception&)
				{
					cout << "server returned invalid message or someone won and the end game function has not been implemented yet" << endl;
					//return -1;
				}
			}

			waitingFor = 1;
		}
		else if (rlobby.curr_player == userID & rlobby.game_status == "f") {
			historyId = cnn.AcknowledgeEnd(lobbyID, userID, 1);

			cout << "historyID: " << historyId << endl;
			cout << "GAME OVER, WINNER IS " << cnn.GetWinner(historyId) << endl;

			ClearCatche(historyId,Shots,Grid,waitingFor);

			langas = 2;
		}
		else if (rlobby.curr_player == userID & rlobby.game_status == "e") {

			cout << "GAME CRASHED" << endl;
			langas = 1;
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
					sf::Vector2i gridPosition = GetGridPosition(x, y, sf::Vector2f(graphics["area1"].getSize().x * 0.1f * ((float)sc.NewWidth / (float)sc.width), graphics["area1"].getSize().y * 0.1f * ((float)sc.NewHeight / (float)sc.height)));//STEADY AND PRECISE AIM
					if (gridPosition.x != -1 && gridPosition.y != -1)
					{
						//Šuvis GO!
						if (TryToMakeAShot(gridPosition, Shots[0], Grid[0]))
						{
							userInput = std::to_string((int)gridPosition.x) + "-" + std::to_string((int)gridPosition.y);
							cout << "myMove: " << userInput << endl;
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
			cout << "result: " << DidYouMakeIt << endl; // mes negalim atvaizduoti pataike ar nepataike... but we can :)
			cnn.WriteMove(lobbyID, "");//inform the server that we got the answer
			waitingFor = 0;
		}
		break;

	default:
		break;
	}

	//Piesimas
	window.clear();
	window.draw(graphics["background"]);
	window.draw(graphics["area1"]);
	window.draw(graphics["area2"]);

	for (int i = 0; i < TableWidth; i++) {
		for (int j = 0; j < TableHeight; j++) {
			window.draw(Grid[0][i][j]);
			window.draw(Grid[1][i][j]);
		}
	}

	for (int i = 0; i < LaivuCount; i++)//atvaizduojame zaidejo laivus
	{
		window.draw(Laivai[i].GetLaivasRectangle());
	}

	for (int k = 0; k < 2; k++)	//atvaizduojame abieju zaideju suvius 
	{
		for (int i = 0; i < Shots[k].size(); i++)
		{
			window.draw(Shots[k][i].GetShotRect());
		}

	}
	window.display();
}

void NewLobbyScreen(sf::RenderWindow& window, sf::Event& event, map<string, sf::RectangleShape>& graphics, map<string, sf::Text>& texts, int& langas, string& lobbyName) {
	
	int x, y;
	sf::RectangleShape pele;

	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
		if (event.type == sf::Event::Resized)
		{
			sc.NewWidth = event.size.width;
			sc.NewHeight = event.size.height;
		}

		texts["lobbyName"].setString(lobbyName);

		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				x = event.mouseButton.x / ((float)sc.NewWidth / (float)sc.width);
				y = event.mouseButton.y / ((float)sc.NewHeight / (float)sc.height);
				pele.setSize(sf::Vector2f(1.f, 1.f));
				pele.setPosition(sf::Vector2f(x, y));
			}

			if (graphics["back"].getGlobalBounds().intersects(pele.getGlobalBounds())) {
				langas = 2;
			}
		}

		if (event.type == sf::Event::TextEntered)
		{
			if (event.text.unicode == 8) { //jeigu backspace - istriname
				if (lobbyName.size() > 0) {
					lobbyName = lobbyName.substr(0, lobbyName.size() - 1);
					texts["lobbyName"].setString(lobbyName);
				}
			}
			else if (event.text.unicode == 13) { //enter submittina faila
				try
				{
					cout << "Creating lobby with ID: ";
					lobbyID = cnn.CreateLobby(lobbyName, userID);
					cout << lobbyID << endl;
				}
				catch (const char* s)
				{
					cout << "error creating lobby: " << s << endl;
				}
				langas = 3;
			}
			else {
				lobbyName += event.text.unicode;
				texts["lobbyName"].setString(lobbyName);
			}
		}

	}
	window.clear();
	window.draw(graphics["background"]);
	window.draw(texts["lobbyName"]);
	window.draw(graphics["back"]);
	window.display();
}

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

	sc.width = 1920 / 2;
	sc.height = 1920 / 4;
	sc.NewWidth = sc.width;
	sc.NewHeight = sc.height;

	sf::RenderWindow window(sf::VideoMode(sc.width, sc.height), "Laivu musis", sf::Style::Resize | sf::Style::Close);
	window.setFramerateLimit(60);


	//Teksturos 
	map<string, sf::Texture> textures;	//map of all textures used in the application. Select them by name
	ifstream textureFile("Media/Texture_paths.txt");
	string textureName;
	while (textureFile >> textureName) {
		sf::Texture t;
		t.loadFromFile("Media/" + textureName + ".png");

		textures[textureName] = t;
	}
	textureFile.close();



	//Fonts
	map<string, sf::Text> texts;
	map<string, sf::Font> fonts;
	ifstream fontFile("Media/Font_paths.txt");
	string fontName;
	while (fontFile >> fontName) {
		sf::Font t;
		t.loadFromFile("Media/" + fontName + ".ttf");

		fonts[fontName] = t;
	}
	fontFile.close();



	//loading graphics
	map<string, sf::RectangleShape> graphics;
	SetupApplicationGraphics(graphics, textures, texts, fonts);


	vector<vector<sf::RectangleShape>> Grid[2];
	//Suviu kurimas
	vector<Shot> Shots[2];//0 - zaidejas 1- priesininkas

	string mapPath = "./.txt";
	string PlayerMap = "";
	string lobbyName = "Lobby name";
	int waitingFor = 0;
	int langas = 1;
	int historyId = 0;

	while (window.isOpen())
	{
		sf::Event event;
		switch (langas)
		{
		case 0: //game screen
			
			GameScreen(window,event,graphics,texts,langas,historyId,Shots,Grid,textures,waitingFor);
			break;
		case 1: //login screen

			LoginScreen(window, event, langas, graphics, texts);
			break;

		case 2: //lobby list

			LobbyListScreen(window, event, langas, graphics,fonts,texts);
			break;

		case 3: //loading screen game.exe ijungiama, irasome failo pavadinima su 100 skaiciu, reprezentuojanciu musu laivu isdestyma, laukiama patvirtinimo ir pradedamas zaidimas

			LoadingScreen(window, event, graphics, texts, langas,mapPath,PlayerMap,Grid,textures);
			break;
		case 4: //New created lobby screen 
			
			NewLobbyScreen(window,event,graphics,texts,langas,lobbyName);
			break;
		default:
			cout << "why u do dis " << langas << endl;
			return 0;
			break;
		}
	}
	return 0;
}
