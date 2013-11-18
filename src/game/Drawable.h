#ifndef TIMDRAWABLE_H
#define TIMDRAWABLE_H
#include <string>
#include "../general/Settings.h"
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Highlight.h"



//An abstract base type, defines the interface.
class Drawable
{
public:
	Drawable(Highlight* highlight_ptr=NULL) : highlight(highlight_ptr)  {};
	virtual ~Drawable() {
		delete highlight;	
	};
	virtual void draw(sf::RenderWindow&) =0;
	virtual void update(b2Body*) =0;
	virtual void setHighlight(std::string, bool);
protected:
    Highlight* highlight;
};

class PlatformDrawable : public Drawable {
public:
	PlatformDrawable(float x, float y, float width, float height);
	void draw(sf::RenderWindow& win) ;
	void update(b2Body* ptr);
	bool highlightPoint(sf::Vector2i);
	sf::Vector2i highlightDelta(sf::Vector2i);
private:
	sf::ConvexShape polygon;
	bool highlighted;
};


#endif
