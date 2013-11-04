

#ifndef LEVEL_H
#define LEVEL_H
#include <list>
#include <utility>
#include "GameObject.h"
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "DebugDraw.h"

namespace {
    b2Vec2 default_gravity(0.0f, 9.8f);
    float32 timestep = 1.0f/60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;
}



class LevelData
{
public:
	LevelData(sf::RenderWindow& _App) : App(_App), DebugDrawInstance(_App), phys_world(default_gravity), level_loaded(false)
	{
        phys_world.SetDebugDraw(&DebugDrawInstance);
        DebugDrawInstance.SetFlags(b2Draw::e_shapeBit);
    }
    ~LevelData() {
        for (auto &iter : levelobjects) {
            delete iter;
        }
    }
    void loadlevel() {
      //Load a demo level:
      
      //A static block:
      std::pair<float,float> statsize(10.0f, 3.0f);
      
      //B2D:
      b2BodyDef blockbodydef;
      blockbodydef.position.Set(12.0f, 40.0f);
      b2Body* statblock = phys_world.CreateBody(&blockbodydef);
      b2PolygonShape blockbox;
      blockbox.SetAsBox(statsize.first, statsize.second);
      statblock->CreateFixture(&blockbox, 0.0f);
      //SFML:
      sf::RectangleShape statblock_sprite;
      sf::Vector2f statspritesize(statsize.first * 2 * 10, statsize.second * 2 * 10);
      statblock_sprite.setSize(statspritesize);
      statblock_sprite.setFillColor(sf::Color::White);
      
      
      //A moving block:
      b2BodyDef bodyDef;
      bodyDef.type = b2_dynamicBody;
      bodyDef.position.Set(22.5f, 20.0f);
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
      sf::Vector2f dynspritesize(2.0f * 10, 2.0f * 10); //Here, the Box2D size is half of the box2D side size.
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
    
    
    void draw(bool debug=false, bool drawsfml=true) {
      if (debug)
	phys_world.DrawDebugData();
      
      if (drawsfml) {
      	for (auto &iter : levelobjects) {
		iter->update_sprite();
		App.draw(*(iter->sprite_ptr));
      	}
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
    sf::RenderWindow& App;
    DebugDraw DebugDrawInstance;
    
    bool level_loaded;
    std::list<sf::RectangleShape> sprites;      //Just for the demo
    
    std::list<GameObject* > levelobjects;	//The level itself
    std::list<GameObject* > playerobjects;	//The objects that the player has placed
    
    
    
};
#endif //LEVEL_H
