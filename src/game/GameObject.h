#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <string>
#include "../general/Settings.h"
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Drawable.h"
#include <vector>
#include "PhysBody.h"
#include <list>

//A Base type, that also defines the interface for drawing.
class GameObject
{

public:
    GameObject(b2World& world, float x, float y, std::string name, Drawable* drawable= new Drawable) : 
        can_place(false), 
        highlight_extras(false), 
        moveStartLocation(0.0, 0.0), 
        local_mouse(x,y),
        position(x, y),
        world(world), 
        name(name), 
        drawable(drawable),
        id(0),
        PLATFORM_THRESHOLD(0.05f),
        DISCRETE_MOVE_UNIT(1.0f),
		teleporttarget(0,0),
		teleportSet(false)
        {};
	virtual ~GameObject(); 
    virtual void update_drawable(); 
    virtual void draw(sf::RenderWindow&);
	virtual void setHighlight(std::string );
	
	// Returns original position
	virtual b2Vec2 getPos() const;
	// Used by WinCondition to get current body position
	virtual b2Vec2 getCurrentPos() const;
	virtual std::string getName(void) const;
	virtual int getID() const;
	virtual void setID(int);
	
	
	virtual void reset();

	virtual void explode() {}
	virtual bool explodeStatus() const { return false; }
	
	virtual bool isDestructable() const { return false; }
	
	//Returns true if the body is not overlapping and false otherwise. 
	virtual bool noOverlaps() const;
	bool can_place;	//This is just so we don't always have to calculate noOverlaps(). Returns the same.

    void setMoveStartLocation(float x, float y);
    void setManipulationStartLocation(sf::Vector2i pos);
	virtual void move(float x, float y);
	virtual void teleport(b2Vec2);

	//Checking wether a point is inside the GameObject
	//It will also change the local_transform so that dragging looks better.
	virtual bool isInside(float x, float y);

	//Wether there are options like rezise, etc.
	//GameObjects that have this as true should also implement highlightDelta etc, probably. 
	bool highlight_extras; 
	

	//Interface functions for stuff like resize, etc.
	virtual bool highlightPoint(sf::Vector2i); 
	virtual void highlightDelta(sf::Vector2i);
	virtual std::string highlightClicked(sf::Vector2i); 


protected:
    
    void moveDiscretely(float x, float y);

	std::vector<PhysBody> bodies;
    b2Vec2 moveStartLocation;
    b2Vec2 manipulationReferenceLocation;
	b2Vec2 local_mouse;
    b2Vec2 position;
	b2World& world;
	std::string name; //Needed for at least LevelData::deletePlayerObject
	Drawable* drawable;
	int id;
    
    const float PLATFORM_THRESHOLD;
    const float DISCRETE_MOVE_UNIT;
	b2Vec2 teleporttarget;
	bool teleportSet;
	
	
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
		Ball(b2World& world, float x, float y, std::string name, float r, float restitution, float density, Drawable* drawable);
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

class Bomb : public GameObject
{
	public:
		Bomb(b2World& world, float x, float y);
		void applyImpulse(b2Body* body, b2Vec2 blastCenter, b2Vec2 applyPoint, float blastPower);
		void explode();
		bool explodeStatus() const;
		bool isDestructable() const;
		void reset();
		void update_drawable();
	private:
		bool exploded;
};

class Crate : public GameObject
{
	public:
		Crate(b2World& world, float x, float y);
};

class Platform : public GameObject
{
	public:
		Platform(b2World& world, float x, float y, float width, float heigth);
		void highlightDelta(sf::Vector2i);
        virtual void move(float x, float y);
        b2Vec2 getDimensions();
    protected:
        b2Vec2 dimensions;
};

class Wall : public GameObject
{
	public:
		Wall(b2World& world, float x, float y, float width, float heigth);
		void highlightDelta(sf::Vector2i);
        virtual void move(float x, float y);
        b2Vec2 getDimensions();
    protected:
        b2Vec2 dimensions;
};

class Seesaw : public GameObject
{
	public:	
		Seesaw(b2World& world, float x, float y, bool flipped= false);
		std::string highlightClicked(sf::Vector2i point);
		bool flipped;
};

class Catapult : public GameObject
{
	public:
		Catapult(b2World& world, float x, float y, bool flipped= false);
		std::string highlightClicked(sf::Vector2i point);
		bool flipped;
	private:
		b2FixtureDef fixtureDef2;
		
};

class GravityChanger : public GameObject
{
	public:
		GravityChanger(b2World& world, float x, float y, bool flipped=false);
		void buttonCheck(b2Fixture*, b2Fixture*);	
		std::string highlightClicked(sf::Vector2i);
		bool flipped;
	private:
		b2Fixture* check1;
		b2Fixture* check2;
};

class MyQueryCallback : public b2QueryCallback {
	public:
		std::vector<b2Body*> foundBodies;

		bool ReportFixture(b2Fixture* fixture) {
			foundBodies.push_back( fixture->GetBody() );
			std::cout << "object\n";
			return true;//keep going to find all fixtures in the query area
		}
};
class Lift : public GameObject
{
	public:
		Lift(b2World& world, float x1, float y1, float x2, float y2);
};

class Teleport : public GameObject
{
public:
	Teleport(b2World& world, float x, float y);
	~Teleport();
	GameObject* calculate_next();	
	void beginContact(GameObject*,b2Fixture*);
	void endContact(GameObject*,b2Fixture*);
	void notify(GameObject*); //Tells the Teleport that this object has just jumped.
private:
	static std::list<GameObject* > teleports; //List of all teleports for deciding where to jump.
	std::list<GameObject* > incoming; //While a pointer is here, the object cannot jump. After endContact() it is removed.
	b2Fixture* my_check;
		
};
/*
class Chain : public GameObject
{
	public:
		Chain(b2World& world);
};
*/
#endif //GAMEOBJECT_H


