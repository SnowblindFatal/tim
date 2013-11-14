#include "GameObject.h"
#include <SFML/Graphics.hpp>
#include <string>

GameObject* GameObjectFactory(b2World& world, std::string name, float x, float y) {
	if (name=="Platform")
		return new Platform(world, x,y, 20.0f, 0.0f);
	if (name=="Wall")
		return new Wall(world, x,y, 0.0f, 20.0f);
	return NULL; //Name not found!
}
/* TO BE REMOVED
void HorizontalBlock::update_drawable() {
	drawable.update(body_ptr);
}

void DroppingSquare::update_drawable() {
	drawable.update(body_ptr);
}
*/

Domino::Domino(b2World& world, float x, float y) : GameObject(x,y) {
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

Platform::Platform(b2World& world, float x, float y, float width, float heigth) : GameObject(x,y) {
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

Wall::Wall(b2World& world, float x, float y, float width, float heigth) : GameObject(x,y) {
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

Ball::Ball(b2World& world, float x, float y, float r, float restitution = 0, float density = 1.0) : GameObject(x,y) {
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

BouncingBall::BouncingBall(b2World& world, float x, float y) : Ball(world, x, y, 0.5, 0.5, 1.0) {}

BowlingBall::BowlingBall(b2World& world, float x, float y) : Ball(world, x, y, 0.8, 0.1, 3.0) {}

BigBall::BigBall(b2World& world, float x, float y) : Ball(world, x, y, 2.0, 0.1, 0.4) {}

/*
Chain::Chain(b2World& world) : GameObject(10,10) {
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
	jointDef.localAnchorA.Set(0.5, 0);
	jointDef.localAnchorB.Set(-0.5, 0);
	for (int i=0; i<40; i++) {
		bodyDef.position.Set(10 + i*1, 10);
		b2Body* link = world.CreateBody(&bodyDef);
		link->CreateFixture(&fixtureDef);
		jointDef.bodyA = body_ptr;
		jointDef.bodyB = link;
		world.CreateJoint(&jointDef);
		body_ptr = link;
	}
	bodyDef.position.Set(10 + 41, 10);
	body_ptr = world.CreateBody(&bodyDef);
	b2CircleShape circleShape;
	circleShape.m_p.Set(0, 0);
	circleShape.m_radius = 1;
	fixtureDef.shape = &circleShape;
	fixtureDef.density = 5;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 0;
	body_ptr->CreateFixture(&fixtureDef);
	jointDef.bodyA = body_ptr;
	jointDef.bodyB = link;
	world.CreateJoint(&jointDef);
}
*/
