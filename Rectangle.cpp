#include "Rectangle.hpp"

#define SCALE 100
Rectangle::Rectangle():
	rectangle()
{
}

Rectangle::Rectangle(sf::Vector2i _startPosition):
	rectangle()
{
	startPosition = _startPosition;
}


Rectangle::~Rectangle()
{
}

sf::Drawable& Rectangle::returnDrawable()
{
	return rectangle;
}
void Rectangle::change(sf::Vector2i _currentPosition)
{
	int A = _currentPosition.x - startPosition.x;
	int B = _currentPosition.y - startPosition.y;
	
	rectangle.setOrigin(abs(A/2), abs(B/2));
	rectangle.setSize(sf::Vector2f(abs(A), abs(B)));
	rectangle.setPosition(startPosition.x + A/2, startPosition.y + B/2 );
}
void Rectangle::update()
{
	if(body != nullptr)
	{
		rectangle.setPosition(body->GetPosition().x * SCALE , -(body->GetPosition().y * SCALE));
		rectangle.setRotation(-body->GetAngle()*180/M_PI);
	}
}
void Rectangle::setColor()
{
	int i = std::rand()%20;
	rectangle.setFillColor(sf::Color(colors[i][0], colors[i][1], colors[i][2]));
}

void Rectangle::setSelected()
{
	rectangle.setOutlineThickness(4);
	rectangle.setOutlineColor(sf::Color::Black);
}

void Rectangle::unsetSelected()
{
	rectangle.setOutlineThickness(0);
}