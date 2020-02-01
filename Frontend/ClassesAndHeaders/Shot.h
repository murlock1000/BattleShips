#pragma once
#include <SFML/Graphics/RectangleShape.hpp>

class Shot
{
public:
	sf::Vector2i gridPosition;
	sf::RectangleShape ShotRect;
	Shot(sf::Vector2i, sf::RectangleShape);
	sf::RectangleShape GetShotRect();
};