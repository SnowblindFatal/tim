#ifndef TIMDRAWABLE_H
#define TIMDRAWABLE_H
#include <string>
#include "../general/Settings.h"
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <iostream>



//An abstract base type, defines the interface.
class Drawable
{
public:
	Drawable() {};
	virtual ~Drawable() {};
	virtual void draw(sf::RenderWindow&) =0;
	virtual void update(b2Body*) =0;
	virtual void setHighlight(std::string , bool ) {};
};

class PlatformDrawable : public Drawable {
public:
	PlatformDrawable(float x, float y, float width, float height);
	void draw(sf::RenderWindow& win) ;
	void update(b2Body* ptr);
	void setHighlight(std::string active, bool can_place=false);
private:
	sf::ConvexShape polygon;
	sf::RectangleShape highlight;
	bool highlighted;
};

#endif
