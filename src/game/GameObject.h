

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <string>
#include "../general/Settings.h"
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <iostream>


//A Base type, that also defines the interface for drawing.
class GameObject
{

public:
    GameObject() {};
	virtual ~GameObject() {};
    virtual void update_drawable() {};
    virtual void draw(sf::RenderWindow&) const {};
protected:
	b2Body* body_ptr;
};	

//An abstract base type, defines the interface.
class Drawable
{
public:
	Drawable() {};
	virtual ~Drawable() {};
	virtual void draw(sf::RenderWindow&) const =0;
	virtual void update(b2Body*) =0;
};



/* HorizontalBlock:
	A static block that can be of any length.
	It can be rotated to 45 degree angle. (Unimplemented)
*/


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
	HorizontalBlock(b2World& world, float x, float y, float length=10.0f, float angle=0.0f) : GameObject(), drawable(x,y,length,angle) {
		
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


/* DroppingSquare:
	A dynamic square of varying size.
*/

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
	DroppingSquare(b2World& world, float x, float y, float length=1.0f, float angle=0.0f) : GameObject(), drawable(x,y,length,angle) {
		
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

/*
Now drawing is separated from everything else.
ExampleSquare: DroppingSquare without the drawing.
You can still see it in the with the debug draw (press D).
*/

class ExampleSquare : public GameObject
{
public:
	ExampleSquare(b2World& world, float x, float y, float length=1.0f) : GameObject() {
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
	



#endif //GAMEOBJECT_H

