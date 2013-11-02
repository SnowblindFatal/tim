

#ifndef LEVEL_H
#define LEVEL_H
#include <list>
#include "GameObject.h"
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

namespace {
    b2Vec2 default_gravity(0.0f, 9.8f);
    float32 timestep = 1.0f/60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;
}

class LevelData
{
public:
	LevelData() : phys_world(default_gravity), level_loaded(false) {}
    ~LevelData() {
        for (auto &iter : levelobjects) {
            delete iter;
        }
    }
	void loadlevel() {
        //Load a demo level:
        //NOTE: The sprites and bodies do not match yet.
        
        
        
        //A static block:
        
        //B2D:
        b2BodyDef blockbodydef;
        blockbodydef.position.Set(12.0f, 40.0f);
        b2Body* statblock = phys_world.CreateBody(&blockbodydef);
        b2PolygonShape blockbox;
        blockbox.SetAsBox(10.0f, 3.0f);
        statblock->CreateFixture(&blockbox, 0.0f);
        //SFML:
        sf::RectangleShape statblock_sprite;
        sf::Vector2f statspritesize(100.0f, 30.0f);
        statblock_sprite.setSize(statspritesize);
        statblock_sprite.setFillColor(sf::Color::White);
        
        
        //A moving block:
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(12.0f, 20.0f);
        b2Body* dynblock = phys_world.CreateBody(&bodyDef);
        b2PolygonShape dynblockbox;
        dynblockbox.SetAsBox(1.0f, 1.0f);
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynblockbox;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;
        dynblock->CreateFixture(&fixtureDef);
        //SFML:
        sf::RectangleShape dynblock_sprite;
        sf::Vector2f dynspritesize(40.0f, 40.0f);
        dynblock_sprite.setSize(dynspritesize);
        dynblock_sprite.setFillColor(sf::Color::Red);
        
        
        //Must figure out a better way to store sprites and GameObjects, this is just for the demo
        sprites.push_back(statblock_sprite);
        sprites.push_back(dynblock_sprite);
        auto sprite_it = sprites.begin();
        levelobjects.push_back(new GameObject("Static", statblock, &(*sprite_it)));
        sprite_it++;
        levelobjects.push_back(new GameObject("Dynamic", dynblock, &(*sprite_it)));
        
        level_loaded = true;
	}
    
    
	void draw(sf::RenderWindow& win) {
        for (auto &iter : levelobjects) {
            iter->update_sprite();
            win.draw(*(iter->sprite_ptr));
        }
    }
	
    
    //One Box2D step:
	void simulate() {
        phys_world.Step(timestep, velocityIterations, positionIterations);
    }
    
    
	bool loaded(void) const {
        return level_loaded;
    }
private:
	
	b2World phys_world;
    
    
    bool level_loaded;
	std::list<sf::RectangleShape> sprites;      //Just for the demo
    
    std::list<GameObject* > levelobjects;		//The level itself
	std::list<GameObject* > playerobjects;		//The objects that the player has placed

};
#endif //LEVEL_H
