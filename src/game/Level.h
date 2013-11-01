

#ifndef LEVEL_H
#define LEVEL_H
#include <list>
#include "GameObject.h"
#include <Box2D.h>

namespace {
b2Vec2 default_gravity(0.0f, 9.8f);
}

class LevelData
{
public:
	LevelData() : phys_world(default_gravity, true) {}
	void loadlevel() {
	//Load a demo level:
	
	//A static block:
	b2BodyDef blockbodydef;
	blockbodydef.position.Set(10, 10);
	b2Body* statblock = phys_world.CreateBody(&blockbodydef);
	b2PolygonShape blockbox;
	blockbox.SetAsBox(10.0f, 3.0f);
	statblock->CreateFixture(&blockbox, 0.0f);
	
	//A moving block:
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 4.0f);
	b2Body* dynblock = world.CreateBody(&bodyDef)
	b2PolygonShape dynblockbox;
	dynblockbox.SetAsBox(1.0f, 1.0f);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynblockbox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	dynblock->CreateFixture(&fixtureDef);
	
	}
	
	
	
	
private:
	
	b2World phys_world;
	std::list<sf::Sprite>
	
	std::list<GameObject*> levelobjects;		//The level itself
	std::list<GameObject*> playerobjects;		//The objects that the player has placed

};
#endif //LEVEL_H
