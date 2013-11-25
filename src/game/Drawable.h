#ifndef TIMDRAWABLE_H
#define TIMDRAWABLE_H
#include <string>
#include "../general/Settings.h"
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Highlight.h"



//A base type, defines the interface.
class Drawable
{
public:
	Drawable(Highlight* highlight_ptr=new Highlight) : highlight(highlight_ptr)  {};
	virtual ~Drawable() {
		delete highlight;	
		highlight=NULL;
	};
	virtual void draw(sf::RenderWindow&) {} 
	virtual void update(b2Body*) {} 
	virtual void setHighlight(std::string, bool);
	virtual std::string highlightClicked(sf::Vector2i);
	virtual sf::Vector2i highlightDelta(sf::Vector2i);
	bool highlightPoint(sf::Vector2i);
protected:
    Highlight* highlight;
};

class PlatformDrawable : public Drawable {
public:
	PlatformDrawable(float x, float y, float width, float height);
	void draw(sf::RenderWindow& win) ;
	void update(b2Body* ptr);
private:
	sf::ConvexShape polygon;
};


#endif
