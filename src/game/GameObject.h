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
    GameObject(float x, float y, float rotation=0.0f) : can_place(false), original_pos(x,y), original_rot(rotation), local_mouse(0,0) {};
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


protected:

	b2Body* body_ptr;
	b2Vec2 original_pos;
	float original_rot;
	b2Vec2 local_mouse;
};	

	

//A Function that returns a pointer to a dynamically allocated GameObject of
//corresponding name, at location x, y
GameObject* GameObjectFactory(b2World& world, std::string name, float x, float y);



/* TO BE REMOVED
 HorizontalBlock:
	A static block that can be of any length.
	It can be rotated to 45 degree angle. (Unimplemented)



class HorizontalBlockDrawable : public Drawable
{

public:
	HorizontalBlockDrawable(float x, float y, float length, float angle) {
		
		sf::Vector2f spritesize(length * 2 * 10, 2 * 10); //There's a factor of two, because B2D takes half side sizes
		sprite.setSize(spritesize);
		sprite.setFillColor(sf::Color::White);
		sprite.setPosition(x*10,y*10);
		sprite.setRotation(180.0f*angle/3.14159f);
		sprite.setOrigin(sprite.getSize().x/2, sprite.getSize().y/2);
	}
	~HorizontalBlockDrawable() {};
	
	void update(b2Body* body_ptr) {
		float x = body_ptr->GetPosition().x;
		float y = body_ptr->GetPosition().y; 
		float angle = body_ptr->GetAngle();
		sprite.setPosition(x*10,y*10);
		sprite.setRotation(180.0f*angle/3.14159f);
		sprite.setOrigin(sprite.getSize().x/2, sprite.getSize().y/2);
	}

	void draw(sf::RenderWindow& target)  const {
		target.draw(sprite);
	}
private:
	sf::RectangleShape sprite;
};

class HorizontalBlock : public GameObject
{
public:
	HorizontalBlock(b2World& world, float x, float y, float length=10.0f, float angle=0.0f) : GameObject(x,y), drawable(x,y,length,angle) {
		
		//B2D:
		b2BodyDef blockbodydef;
		blockbodydef.position.Set(x, y);
		body_ptr = world.CreateBody(&blockbodydef);
		b2PolygonShape blockbox;
		blockbox.SetAsBox(length, 1.0f);
		body_ptr->CreateFixture(&blockbox, 0.0f);

	}
	~HorizontalBlock() {};
	void update_drawable();
	void draw(sf::RenderWindow& win) const{
		drawable.draw(win);
	}

private:
	HorizontalBlockDrawable drawable;
};	


DroppingSquare:
	A dynamic square of varying size.


class DroppingSquareDrawable : public Drawable
{
public:
	DroppingSquareDrawable(float x, float y, float side, float angle) {
		
		sf::Vector2f spritesize(side * 2 * 10, side  * 2 *10);
		sprite.setSize(spritesize);
		sprite.setFillColor(sf::Color::Red);
		sprite.setPosition(x*10,y*10);
		sprite.setRotation(180.0f*angle/3.14159f);
		sprite.setOrigin(sprite.getSize().x/2, sprite.getSize().y/2);
	}
	
	void update(b2Body* body_ptr) {
		float x = body_ptr->GetPosition().x;
		float y = body_ptr->GetPosition().y; 
		float angle = body_ptr->GetAngle();
		sprite.setPosition(x*10,y*10);
		sprite.setRotation(180.0f*angle/3.14159f);
		sprite.setOrigin(sprite.getSize().x/2, sprite.getSize().y/2);
	}
	void draw(sf::RenderWindow& target)  const {
		target.draw(sprite); 
	}
	
private:
	sf::RectangleShape sprite;
};

	
class DroppingSquare : public GameObject
{
public:
	DroppingSquare(b2World& world, float x, float y, float length=1.0f, float angle=0.0f) : GameObject(x,y), drawable(x,y,length,angle) {
		
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(x, y);
		body_ptr = world.CreateBody(&bodyDef);
		b2PolygonShape dynblockbox;
		dynblockbox.SetAsBox(length, length);
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynblockbox;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.3f;
		body_ptr->CreateFixture(&fixtureDef);

	}
	~DroppingSquare() {};

	void update_drawable();
	void draw(sf::RenderWindow& win) const {
		drawable.draw(win);
	}


private:
	DroppingSquareDrawable drawable;
};


Now drawing is separated from everything else.
ExampleSquare: DroppingSquare without the drawing.
You can still see it in the with the debug draw (press D).


class ExampleSquare : public GameObject
{
public:
	ExampleSquare(b2World& world, float x, float y, float length=1.0f) : GameObject(x,y) {
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(x, y);
		body_ptr = world.CreateBody(&bodyDef);
		b2PolygonShape dynblockbox;
		dynblockbox.SetAsBox(length, length);
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynblockbox;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.3f;
		body_ptr->CreateFixture(&fixtureDef);
	}
	~ExampleSquare() {};
};
*/

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
	private:
		PlatformDrawable drawable;
};

class Wall : public GameObject
{
	public:
		Wall(b2World& world, float x, float y, float width, float heigth);
};


class Chain : public GameObject
{
	public:
		Chain(b2World& world);
};


#endif //GAMEOBJECT_H

