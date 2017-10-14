#ifndef POLYGON_HPP
#define POLYGON_HPP

#include "Entity.hpp"
#include <iostream>
#include <cmath>
class Polygon: public Entity
{
private:
	sf::ConvexShape polygon;
	sf::VertexArray lines;
	sf::Vector2i startPosition;
	
	struct IntersectionStruct{
		sf::Vector2f point;
		int edge;
	};
	
	int det (int a, int b, int c, int d); 
	bool between (int a, int b, double c); 
	bool intersect_1 (int a, int b, int c, int d);
	sf::Vector2f intersect (sf::Vector2f a, sf::Vector2f b, sf::Vector2f c, sf::Vector2f d);
	
public:
	Polygon();
	Polygon(sf::Vector2i _startPosition);
	~Polygon();
	sf::Drawable& returnDrawable();
	
	void change(sf::Vector2i _currentPosiotion);
	void update();
	void setColor();
	void newPolygon();
	void setSelected();
	void unsetSelected();

};

#endif // POLYGON_HPP
