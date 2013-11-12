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
    GameObject(float x, float y, float rotation=0.0f) : original_pos(x,y), original_rot(rotation) {};
	virtual ~GameObject() {};
    virtual void update_drawable() {};
    virtual void draw(sf::RenderWindow&) const {};

	virtual b2Vec2  getPos() const {
		return body_ptr->GetPosition();
	}
	virtual void reset() {
		body_ptr->SetTransform(original_pos, original_rot);
		body_ptr->SetLinearVelocity(b2Vec2(0,0));
		body_ptr->SetAngularVelocity(0);
	}

protected:
	b2Body* body_ptr;
	b2Vec2 original_pos;
	float original_rot;

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

/*
Now drawing is separated from everything else.
ExampleSquare: DroppingSquare without the drawing.
You can still see it in the with the debug draw (press D).
*/

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

class Domino : public GameObject
{
	public:
		Domino(b2World& world, float x, float y) : GameObject(x,y) {
			b2BodyDef bodyDef;
			bodyDef.type = b2_dynamicBody;
			bodyDef.position.Set(x, y);
			body_ptr = world.CreateBody(&bodyDef);
			b2PolygonShape dominoShape;
			dominoShape.SetAsBox(0.4, 1.5);
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &dominoShape;
			fixtureDef.density = 3.0f;
			fixtureDef.friction = 0.5f;
			body_ptr->CreateFixture(&fixtureDef);
		}
};

/*
class Dominos : public GameObject
{
	public:
		Dominos(b2World world, float x, float y, float length=1.0f) : GameObject(x,y) {
			b2BodyDef bodyDef;
			bodyDef.type = b2_dynamicBody;
			bodyDef.position.Set(x, y);
			body_ptr = world.CreateBody(&bodyDef);
			b2PolygonShape dominoShape;
			dominoShape.SetAsBox(1.0, 1.0);
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &dominoShape;
			fixtureDef.density = 1.0f;
			fixtureDef.friction = 0.3f;
			//fixtureDef.restitution = 0.1f;
			body_ptr->CreateFixture(&fixtureDef);
		}
		~Dominos() {};
};
*/
class Ball : public GameObject
{
	public:
		Ball(b2World& world, float x, float y, float r, float restitution = 0, float density = 1.0) : GameObject(x,y) {
			b2BodyDef bodyDef;
			bodyDef.type = b2_dynamicBody;
			bodyDef.position.Set(x, y);
			body_ptr = world.CreateBody(&bodyDef);	
			b2CircleShape circleShape;
  			circleShape.m_p.Set(0, 0);
  			circleShape.m_radius = r;
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &circleShape;
			fixtureDef.density = density;
			fixtureDef.friction = 0.3f;
			fixtureDef.restitution = restitution;
			body_ptr->CreateFixture(&fixtureDef);
		}
};

class BouncingBall : public Ball
{
	public:
		BouncingBall(b2World& world, float x, float y) : Ball(world, x, y, 0.5, 0.5, 1.0) {}
};

class BowlingBall : public Ball
{
	public:
		BowlingBall(b2World& world, float x, float y) : Ball(world, x, y, 0.8, 0.1, 3.0) {}
};

class BigBall : public Ball
{
	public:
		BigBall(b2World& world, float x, float y) : Ball(world, x, y, 2.0, 0.1, 0.4) {}
};
class Platform : public GameObject
{
	public:
		Platform(b2World& world, float x, float y, float width, float heigth) : GameObject(x,y) {
			b2BodyDef bodyDef;
			bodyDef.position.Set(x, y);
			body_ptr = world.CreateBody(&bodyDef);
			b2Vec2 vertices[4];
			vertices[0].Set(0, 0);
			vertices[1].Set(0, 2);
			vertices[2].Set(width, heigth);
			vertices[3].Set(width, heigth + 2);
			b2PolygonShape polygonShape;
			polygonShape.Set(vertices, 4);
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &polygonShape;
			body_ptr->CreateFixture(&fixtureDef);
		}
};

class Wall : public GameObject
{
	public:
		Wall(b2World& world, float x, float y, float width, float heigth) :GameObject(x,y) {
			b2BodyDef bodyDef;
			bodyDef.position.Set(x, y);
			body_ptr = world.CreateBody(&bodyDef);
			b2Vec2 vertices[4];
			vertices[0].Set(0, 0);
			vertices[1].Set(2, 0);
			vertices[2].Set(width, heigth);
			vertices[3].Set(width +2, heigth);
			b2PolygonShape polygonShape;
			polygonShape.Set(vertices, 4);
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &polygonShape;
			body_ptr->CreateFixture(&fixtureDef);		
		}
};


class Chain : public GameObject
{
	public:
		Chain(b2World& world) : GameObject(10,10) { //This class seems to be incomplete. Had to put 10,10 here.
			b2BodyDef bodyDef;
			bodyDef.type = b2_dynamicBody;
			bodyDef.position.Set(10, 10);
			body_ptr = world.CreateBody(&bodyDef);
			b2PolygonShape chainShape;
			chainShape.SetAsBox(0.5, 0.25);
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &chainShape;
			fixtureDef.density = 1.0f;
			body_ptr->CreateFixture(&fixtureDef);
			b2RevoluteJointDef jointDef;
			jointDef.localAnchorA.Set(0.25, 0);
			jointDef.localAnchorB.Set(-0.25, 0);
			for (int i=0; i<40; i++) {
				bodyDef.position.Set(10 + i*0.5, 10);
				b2Body* link = world.CreateBody(&bodyDef);
				link->CreateFixture(&fixtureDef);
				jointDef.bodyA = body_ptr;
				jointDef.bodyB = link;
				world.CreateJoint(&jointDef);
				body_ptr = link;
			}
		}
};


#endif //GAMEOBJECT_H

