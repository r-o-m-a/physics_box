#include "Circle.hpp"

#define SCALE 100

Circle::Circle():
	circle(10)
{
}

Circle::Circle(sf::Vector2i _startPosition):
	circle()
{
	startPosition = _startPosition;
	
}

Circle::~Circle()
{
}

sf::Drawable& Circle::returnDrawable()
{
	return circle;
}

void Circle::change(sf::Vector2i _currentPosition)
{
	int radius = sqrt(pow((_currentPosition.x - startPosition.x), 2 ) + 
							   pow((_currentPosition.y - startPosition.y), 2 )) / 2;
	circle.setOrigin(radius, radius);
	
	circle.setRadius(radius);
	circle.setPosition((startPosition.x + _currentPosition.x)/2, (startPosition.y + _currentPosition.y)/2 );
}

void Circle::update()
{
	if(body != nullptr)
		circle.setPosition(body->GetPosition().x * SCALE , -(body->GetPosition().y * SCALE));
}

void Circle::setColor()
{
	int i = std::rand()%20;
	circle.setFillColor(sf::Color(colors[i][0], colors[i][1], colors[i][2]));
}

void Circle::setSelected()
{
	circle.setOutlineThickness(4);
	circle.setOutlineColor(sf::Color::Black);
	
}

void Circle::unsetSelected()
{
	circle.setOutlineThickness(0);
}