#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <string>
#include "../general/Settings.h"
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Drawable.h"

//A Base type, that also defines the interface for drawing.
class GameObject
{

public:
    GameObject(float x, float y, float rotation=0.0f) : can_place(false), highlight_extras(false), original_pos(x,y), original_rot(rotation), local_mouse(0,0) {};
	virtual ~GameObject() {}
    virtual void update_drawable() {}
    virtual void draw(sf::RenderWindow&) {}
	virtual void setHighlight(std::string ) {}

	virtual b2Vec2  getPos() const;
	
	virtual void reset();
	
	//Returns true if the body is not overlapping and false otherwise. 
	virtual bool noOverlaps() const;
	bool can_place;	//This is just so we don't always have to calculate noOverlaps(). Returns the same.

	virtual void move(float x, float y);

	//Checking wether a point is inside the GameObject
	//It will also change the local_transform so that dragging looks better.
	virtual bool isInside(float x, float y);

	//Wether there are options like rezise, etc.
	//GameObjects that have this as true should also implement highlightPoint() and highlightDelta()
	bool highlight_extras; 

	//Interface functions for stuff like resize, etc.
	virtual bool highlightPoint(sf::Vector2i) {return false;}
	virtual void highlightDelta(sf::Vector2i) {}
protected:

	b2Body* body_ptr;
	b2Vec2 original_pos;
	float original_rot;
	b2Vec2 local_mouse;
	
};	

	

//A Function that returns a pointer to a dynamically allocated GameObject of
//corresponding name, at location x, y
GameObject* GameObjectFactory(b2World& world, std::string name, float x, float y);



class Domino : public GameObject
{
	public:
		Domino(b2World& world, float x, float y);
};


class Ball : public GameObject
{
	public:
		Ball(b2World& world, float x, float y, float r, float restitution, float density);
};

class BouncingBall : public Ball
{
	public:
		BouncingBall(b2World& world, float x, float y);
};

class BowlingBall : public Ball
{
	public:
		BowlingBall(b2World& world, float x, float y);
};

class BigBall : public Ball
{
	public:
		BigBall(b2World& world, float x, float y);
};
class Platform : public GameObject
{
	public:
		Platform(b2World& world, float x, float y, float width, float heigth);
		void update_drawable();
		void draw(sf::RenderWindow&) ;
		void setHighlight(std::string type);
		bool highlightPoint(sf::Vector2i point);
		void highlightDelta(sf::Vector2i point);
	private:
		PlatformDrawable drawable;
};

class Wall : public GameObject
{
	public:
		Wall(b2World& world, float x, float y, float width, float heigth);
		void update_drawable();
		void draw(sf::RenderWindow&) ;
		void setHighlight(std::string type);
		bool highlightPoint(sf::Vector2i point);
		void highlightDelta(sf::Vector2i point);
	private:
		PlatformDrawable drawable;
};

class Seesaw : public GameObject
{
	public:	
		Seesaw(b2World& world, float x, float y);
		void reset();
	private:
		b2Body* body_ptr2;
};

class Catapult : public GameObject
{
	public:
		Catapult(b2World& world, float x, float y);
	private:
		b2Body* body_ptr2;
		b2Body* body_ptr3;		

};

class Chain : public GameObject
{
	public:
		Chain(b2World& world);
};


#endif //GAMEOBJECT_H

