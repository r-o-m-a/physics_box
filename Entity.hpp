#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <SFML/Window.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <Box2D/Box2D.h>

class Entity
{
protected:
	int colors[20][3] = 
	{{250, 128, 114},
	 {255, 160, 122},
	 {139, 0, 0},
	 {255, 192, 203},
	 {255, 140, 0},
	 {219, 112, 147},
	 {238, 130, 238},
	 {186, 85, 211},
	 {255, 228, 196},
	 {222, 184, 135},
	 {165, 42, 42},
	 {169, 169, 169},
	 {128, 128, 128},
	 {47, 79, 79},
	 {0, 191, 255},
	 {65, 105, 225},
	 {0, 255, 127},
	 {0, 100, 0},
	 {128, 128, 0},
	 {65, 105, 225}
	};
public:
	Entity();
	virtual ~Entity();	
	virtual sf::Drawable& returnDrawable() = 0;
	
	virtual void change(sf::Vector2i _currentPosiotion) = 0;
	virtual void update() = 0;
	virtual void setColor() = 0;
	virtual void setSelected() = 0;
	virtual void unsetSelected() = 0;
	
	b2Body* body;
	bool ready;
	
};

#endif // ENTITY_HPP
