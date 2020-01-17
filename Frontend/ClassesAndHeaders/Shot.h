#pragma once
#include <SFML/Graphics/RectangleShape.hpp>

class Shot
{
public:
	sf::Vector2f gridPosition;
	sf::RectangleShape ShotRect;
	Shot(sf::Vector2f, sf::RectangleShape);
	sf::RectangleShape GetShotRect();
};