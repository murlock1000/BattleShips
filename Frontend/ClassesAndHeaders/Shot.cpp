#include "Shot.h"

Shot::Shot(sf::Vector2i gridposition, sf::RectangleShape suvisRect)
{
	gridPosition = gridposition;
	ShotRect = suvisRect;
	ShotRect.setFillColor(sf::Color::Red);
	ShotRect.setOutlineThickness(0.f);
}

sf::RectangleShape Shot::GetShotRect()
{
	return ShotRect;
}
