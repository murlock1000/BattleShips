#pragma once
#include <SFML/Graphics/RectangleShape.hpp>
#include <vector>
#include <cmath>

class Laivas
{
public:
	sf::Vector2f positionStart;
	sf::Vector2f positionEnd;
	bool parallelWithWidth;
	std::vector<sf::Vector2f> dalys;
	sf::RectangleShape LaivasRect;
	int ilgis;

	Laivas(sf::Vector2f, sf::Vector2f);
	void SetRectangleTexture(sf::Texture&);
	void ConstructRectangle(sf::RectangleShape, sf::Vector2f);
	sf::RectangleShape GetLaivasRectangle();
	int GetLaivasLength();
	bool IsRectangleRotated();
	void RotateRectangle(float);
};

