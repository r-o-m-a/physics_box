#ifndef CIRCLE_HPP
#define CIRCLE_HPP
#include "Entity.hpp"

#include <iostream>

class Circle: public Entity
{
private:
	sf::CircleShape circle;
	sf::Vector2i startPosition;
	
public:
	Circle();
	Circle(sf::Vector2i _startPosition);
	~Circle();
	
	sf::Drawable& returnDrawable();
	
	void change(sf::Vector2i _currentPosiotion);
	void update();
	void setColor();
	void setSelected();
	void unsetSelected();
};

#endif // CIRCLE_HPP
