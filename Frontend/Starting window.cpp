#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include "ClassesAndHeaders/shot.h"
#include "ClassesAndHeaders/Laivas.h"

using namespace std;



int ReadFromFile(ifstream& file)
{
	int mbuffer;
	file >> mbuffer;
	return mbuffer;
}

sf::Vector2f GetGridPosition(int width, int height, int x, int y, sf::Vector2f size)
{
	sf::Vector2f buffer = sf::Vector2f((float)((x - 0.5375 * width) / size.x), 1 + (float)((y - 0.075 * width) / size.y));
	return (buffer.x > 0 && buffer.x < 10 && buffer.y > 0 && buffer.y < 10) ? buffer : sf::Vector2f(-1, -1);
}

int main()
{
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

	//Taisykles - Kintamieji
	const int TableWidth = 10;
	const int TableHeight = 10;
	const int PlayerCount = 2;
	const int LaivuCount = 5;

	//Duomenys - Kintamieji
	bool AI[2] = { true, true }; //Ar 1/2 zaidejas - AI
	int ShipTable[TableHeight][TableWidth][2];
	//[y][x][player]

	//duomenų gavimas, kolkas iš failo, reikia pakeisti
	ifstream Data("Media/data.txt");
	for (int k = 0; k < PlayerCount; k++) {
		AI[k] = ReadFromFile(Data);
		for (int i = 0; i < TableHeight; i++)
		{
			for (int j = 0; j < TableWidth; j++)
			{
				ShipTable[i][j][k] = ReadFromFile(Data);
			}
		}
	}

	//Laivu kurimas
	vector<Laivas> Laivai[PlayerCount];
	for (int k = 0; k < PlayerCount; k++) {
		for (int i = 0; i < TableHeight; i++) {
			for (int j = 0; j < TableWidth; j++) {
				if (ShipTable[i][j][k] != 0)
				{
					int xSize = 0;
					int ySize = 0;
					for (int l = 0; j + l < TableWidth && ShipTable[i][j + l][k] != 0; l++)
					{
						xSize++;
						ShipTable[i][j + l][k] = 0;
					}
					ShipTable[i][j][k] = 1;
					for (int l = 0; i + l < TableHeight && ShipTable[i + l][j][k] != 0; l++)
					{
						ySize++;
						ShipTable[i + l][j][k] = 0;
					}
					Laivai[k].push_back(Laivas(sf::Vector2f((float)j, (float)i), sf::Vector2f((float)(j + xSize - 1), (float)(i + ySize - 1))));

				}
			}
		}
	}

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
		Textures[i-2].loadFromFile("Media/Laivas_"+ std::to_string(i) + ".png");
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

	while (window.isOpen())
	{
		//Eventu Handlinimas
		sf::Event event;
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
					sf::Vector2f gridPosition = GetGridPosition(NewWidth, NewHeight, x, y, sf::Vector2f(Area2.getSize().x * 0.1f * ((float)NewWidth/(float)width), Area2.getSize().y * 0.1f * ((float)NewHeight/(float)height)));
					if (gridPosition.x != -1 && gridPosition.y != -1)
					{
						//Šuvis
						Shots[0].push_back(Shot(gridPosition, Grid2[int(gridPosition.x)][int(gridPosition.y)]));
					}
				}
			}
			if (event.type == sf::Event::Resized)
			{
				NewWidth = event.size.width;
				NewHeight = event.size.height;
			}
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

	}

	return 0;
}