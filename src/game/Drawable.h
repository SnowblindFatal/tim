#ifndef TIMDRAWABLE_H
#define TIMDRAWABLE_H
#include <string>
#include "../general/Settings.h"
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Highlight.h"
#include <vector>
#include "PhysBody.h"



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
	virtual void update(const std::vector<PhysBody>&) {} 
	virtual void setHighlight(std::string, bool);
	virtual std::string highlightClicked(sf::Vector2i);
	bool highlightPoint(sf::Vector2i);
protected:
    Highlight* highlight;
};

class PlatformDrawable : public Drawable {
public:
	PlatformDrawable(float x, float y, float width, float height);
	void draw(sf::RenderWindow& win);
	void update(const std::vector<PhysBody>&);
private:
	sf::ConvexShape polygon;
};

class BombDrawable : public Drawable {
public:
	BombDrawable(float x, float y);
	void draw(sf::RenderWindow& win);
	void update(const std::vector<PhysBody>&);
private:
	sf::RectangleShape box;

};

class GravityChangerDrawable : public Drawable {
public:
	GravityChangerDrawable(float x, float y);
	void draw(sf::RenderWindow&);
	void update(const std::vector<PhysBody>&);
private:
	sf::RectangleShape box;
	sf::RectangleShape button;
};

class BowlingBallDrawable : public Drawable {
public:
	BowlingBallDrawable(float x, float y);
	void draw(sf::RenderWindow&);
	void update(const std::vector<PhysBody>&);
private:
	sf::CircleShape ball;
};

class BouncingBallDrawable : public Drawable {
public:
	BouncingBallDrawable(float x, float y);
	void draw(sf::RenderWindow&);
	void update(const std::vector<PhysBody>&);
private:
	sf::CircleShape ball;	
};

class SeesawDrawable : public Drawable {
public:
	SeesawDrawable(float x, float y);
	void draw(sf::RenderWindow&);
	void update(const std::vector<PhysBody>&);
private:
	sf::RectangleShape box;	
	sf::ConvexShape polygon;
};

class DominoDrawable : public Drawable {
public:
	DominoDrawable(float x, float y);
	void draw(sf::RenderWindow&);
	void update(const std::vector<PhysBody>&);
private:
	sf::RectangleShape box;	
};

#endif
