#pragma once
#include <sfml/Graphics/RectangleShape.hpp>

class Shot
{
private:
	sf::Vector2f gridPosition;
	sf::RectangleShape ShotRect;
public:
	Shot(sf::Vector2f, sf::RectangleShape);
	sf::RectangleShape GetShotRect();
};

