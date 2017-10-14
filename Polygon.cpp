#include "Polygon.hpp"
#define SCALE 100

const double EPS = 1E-9;

Polygon::Polygon():
	polygon()
{
}

Polygon::Polygon(sf::Vector2i _startPosition):
	polygon(),
	lines(sf::LinesStrip, 2)
{
	startPosition = _startPosition;
	lines[0].position = static_cast<sf::Vector2f>(startPosition);
	lines[1].position = lines[0].position;
}

Polygon::~Polygon()
{
}

sf::Drawable& Polygon::returnDrawable()
{
	if(ready)
		return polygon;
	else
		return lines;
}
	
void Polygon::change(sf::Vector2i _currentPosition)
{
	if(!ready)
	{
		int i = lines.getVertexCount() -1;
		lines[i].position = static_cast<sf::Vector2f>( _currentPosition);
	}
}

void Polygon::newPolygon()
{
	if(!ready)
	{
		int i = lines.getVertexCount() -1;
		lines[i].color = lines[i-1].color;
		sf::Vector2f intersection;
		if(i > 2)
		{
			std::vector<IntersectionStruct> intersectionVector;
			for(int j = 0; j < i - 2; j++)
			{
				intersection = intersect(lines[j].position, lines[j+1].position, lines[i-1].position, lines[i].position);
				if (intersection.x != 0)
				{
					IntersectionStruct s;
					s.point = intersection;
					s.edge = j+1;
					intersectionVector.push_back(s);
				}
			}
			if(!intersectionVector.empty())
			{
				float min = 10000;
				auto minEdge = intersectionVector.begin();
				for(auto it = intersectionVector.begin(); it != intersectionVector.end(); it++)
				{
					float distance = sqrt(pow(((*it).point.x - lines[i-2].position.y), 2 ) + 
								   pow(((*it).point.x - lines[i-2].position.y), 2 ));
					if(distance < min)
					{
						min = distance;
						minEdge = it;
					}
				}
				int pointCount = lines.getVertexCount() - (*minEdge).edge;
				
				float def = ((*minEdge).point.x - lines[pointCount-2].position.x)*
					(lines[(*minEdge).edge].position.y - (*minEdge).point.y) - 
					((*minEdge).point.y - lines[pointCount-2].position.y)*
					(lines[(*minEdge).edge].position.x - (*minEdge).point.x);
				int kek = 1;
				for(int i = 0; i < pointCount - 1; i++)
				{
					float def2;
					if (i != (pointCount - 2))
						def2 = (lines[(*minEdge).edge + i].position.x - lines[(*minEdge).edge + i - 1].position.x)*
							(lines[(*minEdge).edge + i + 1].position.y - lines[(*minEdge).edge + i].position.y) - 
							(lines[(*minEdge).edge + i].position.y - lines[(*minEdge).edge + i - 1].position.y)*
							(lines[(*minEdge).edge + i + 1].position.x - lines[(*minEdge).edge + i].position.x);
					else
						def2 = (lines[(*minEdge).edge + i].position.x - lines[(*minEdge).edge + i - 1].position.x)*
							((*minEdge).point.y - lines[(*minEdge).edge + i].position.y) - 
							(lines[(*minEdge).edge + i].position.y - lines[(*minEdge).edge + i - 1].position.y)*
							((*minEdge).point.x - lines[(*minEdge).edge + i].position.x);
					if(((def2 >= 0) && (def>= 0)) ||
						((def2 <= 0) && (def<= 0)))
							kek++;
				}
				polygon = sf::ConvexShape(kek);
				polygon.setPoint(0, (*minEdge).point);
				int j = 1;
				for(int i = 0; i < pointCount - 1; i++)
				{
					float def2;
					if (i != (pointCount - 2))
						def2 = (lines[(*minEdge).edge + i].position.x - lines[(*minEdge).edge + i - 1].position.x)*
							(lines[(*minEdge).edge + i + 1].position.y - lines[(*minEdge).edge + i].position.y) - 
							(lines[(*minEdge).edge + i].position.y - lines[(*minEdge).edge + i - 1].position.y)*
							(lines[(*minEdge).edge + i + 1].position.x - lines[(*minEdge).edge + i].position.x);
					else
						def2 = (lines[(*minEdge).edge + i].position.x - lines[(*minEdge).edge + i - 1].position.x)*
							((*minEdge).point.y - lines[(*minEdge).edge + i].position.y) - 
							(lines[(*minEdge).edge + i].position.y - lines[(*minEdge).edge + i - 1].position.y)*
							((*minEdge).point.x - lines[(*minEdge).edge + i].position.x);
					if(((def2 >= 0) && (def>= 0)) ||
						((def2 <= 0) && (def<= 0)))
						{
							polygon.setPoint(j, lines[(*minEdge).edge + i].position);
							j++;
						}
				}
				polygon.setFillColor(lines[0].color);
				lines.resize(0);
				ready = true;
			}
		}
	}
	if(!ready)
		lines.append(sf::Vertex(lines[lines.getVertexCount() -1].position));
}

void Polygon::update()
{
	if(body != nullptr)
	{
		polygon.setPosition(body->GetPosition().x * SCALE , -(body->GetPosition().y * SCALE));
		polygon.setRotation(-body->GetAngle()*180/M_PI);
	}
}
void Polygon::setColor()
{
	int i = std::rand()%20;
	sf::Color color = sf::Color(colors[i][0], colors[i][1], colors[i][2]);
	polygon.setFillColor(color);
	lines[0].color = lines[1].color = color;
}
 
int Polygon::det (int a, int b, int c, int d) {
	return a * d - b * c;
}
 
bool Polygon::between (int a, int b, double c) {
	return std::min(a,b) <= c + EPS && c <= std::max(a,b) + EPS;
}
 
bool Polygon::intersect_1 (int a, int b, int c, int d) {
	if (a > b)  std::swap (a, b);
	if (c > d)  std::swap (c, d);
	return std::max(a,c) <= std::min(b,d);
}
 
sf::Vector2f Polygon::intersect (sf::Vector2f a, sf::Vector2f b, sf::Vector2f c, sf::Vector2f d) 
{
	int A1 = a.y-b.y,  B1 = b.x-a.x,  C1 = -A1*a.x - B1*a.y;
	int A2 = c.y-d.y,  B2 = d.x-c.x,  C2 = -A2*c.x - B2*c.y;
	int zn = det (A1, B1, A2, B2);
	if (zn != 0) {
		double x = - det (C1, B1, C2, B2) * 1. / zn;
		double y = - det (A1, C1, A2, C2) * 1. / zn;
		if (between (a.x, b.x, x) && between (a.y, b.y, y)
			&& between (c.x, d.x, x) && between (c.y, d.y, y))
			return sf::Vector2f(x, y);
		else return sf::Vector2f(0,0);
	}
	else
	{
		if (det (A1, C1, A2, C2) == 0 && det (B1, C1, B2, C2) == 0
			&& intersect_1 (a.x, b.x, c.x, d.x)
			&& intersect_1 (a.y, b.y, c.y, d.y))
				return sf::Vector2f(0, 0);
		else return sf::Vector2f(0, 0);
			
	}
}

void Polygon::setSelected()
{
	polygon.setOutlineThickness(4);
	polygon.setOutlineColor(sf::Color::Black);
}
	
void Polygon::unsetSelected()
{
	polygon.setOutlineThickness(0);
}
	