#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP
#include "Entity.hpp"
#include <iostream>

class Rectangle: public Entity
{
private:
	sf::RectangleShape rectangle;
	sf::Vector2i startPosition;
	
public:
	Rectangle();
	Rectangle(sf::Vector2i _startPosition);
	~Rectangle();
	
	sf::Drawable& returnDrawable();
	
	void change(sf::Vector2i _currentPosiotion);
	void update();
	void setColor();
	void setSelected();
	void unsetSelected();
};

#endif // RECTANGLE_HPP
