#include "Laivas.h"

Laivas::Laivas(sf::Vector2f positionStart, sf::Vector2f positionEnd)
{
	if (positionStart.x < positionEnd.x || positionStart.y < positionEnd.y)
	{
		this->positionStart = positionStart;
		this->positionEnd = positionEnd;
	}
	else
	{
		this->positionStart = positionEnd;
		this->positionEnd = positionStart;
	}
	ilgis = (int)(this->positionEnd.x - this->positionStart.x + this->positionEnd.y - this->positionStart.y + 1);
	parallelWithWidth = (this->positionEnd.x - this->positionStart.x == 0) ? false : true;
	if (parallelWithWidth)
	{
		for (int i = 0; i < ilgis; i++)
		{
			dalys.push_back(sf::Vector2f((float)(positionStart.x + i), (float)(positionStart.y)));
		}
		LaivasRect.setSize(sf::Vector2f((float)ilgis, (float)1));
	}
	else
	{
		for (int i = 0; i < ilgis; i++)
		{
			dalys.push_back(sf::Vector2f((float)(positionStart.x), (float)(positionStart.y + i)));
		}
	}


}

void Laivas::SetRectangleTexture(sf::Texture& textura)
{
	Laivas::LaivasRect.setTexture(&textura);
}

void Laivas::ConstructRectangle(sf::RectangleShape Area, sf::Vector2f gridsize)
{
	float xPosition = Area.getPosition().x +
		this->positionStart.x * gridsize.x;
	float yPosition = Area.getPosition().y + this->positionStart.y * gridsize.y;
	LaivasRect.setPosition(sf::Vector2f(xPosition, yPosition));
	float xSize, ySize;
	if (parallelWithWidth)
	{
		xSize = gridsize.x * this->ilgis;
		ySize = gridsize.y;

	}
	else {
		xSize = gridsize.x;
		ySize = gridsize.y * this->ilgis;
	}
	LaivasRect.setSize(sf::Vector2f(xSize, ySize));

}

sf::RectangleShape Laivas::GetLaivasRectangle()
{
	return LaivasRect;
}

int Laivas::GetLaivasLength()
{
	return ilgis;
}
bool Laivas::IsRectangleRotated()
{
	return !parallelWithWidth;
}
void Laivas::RotateRectangle(float angle)
{
	sf::Vector2f size = LaivasRect.getSize();
	this->LaivasRect.rotate(angle);
	LaivasRect.setSize(sf::Vector2f(size.y, size.x));
	LaivasRect.move(sf::Vector2f(std::min(LaivasRect.getSize().x, LaivasRect.getSize().y), 0.f));
}
