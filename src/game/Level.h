

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
	LevelData(sf::RenderWindow& _App) : phys_world(default_gravity), App(_App), DebugDrawInstance(_App), level_loaded(false)
	{
        phys_world.SetDebugDraw(&DebugDrawInstance);
        DebugDrawInstance.SetFlags(b2Draw::e_shapeBit);
		DebugDrawInstance.AppendFlags(b2Draw::e_centerOfMassBit);
    }
    ~LevelData() {
        for (auto &iter : levelobjects) {
            delete iter;
        }
    }
	void loadlevel() {
      //Load a demo level:
/*
		levelobjects.push_back(new HorizontalBlock(phys_world,12.0f, 40.0f));
		levelobjects.push_back(new DroppingSquare(phys_world,22.5f,20.0f));
		levelobjects.push_back(new DroppingSquare(phys_world,23.0f,18.0f));
		levelobjects.push_back(new DroppingSquare(phys_world,10.0f,20.0f,3.0f));
		levelobjects.push_back(new ExampleSquare(phys_world,15.0f,20.0f));
		levelobjects.push_back(new BouncingBall(phys_world,31.0f,25.0f));
		levelobjects.push_back(new Platform(phys_world, 30.0f, 30.0f, 4.0f, 4.0f));
		levelobjects.push_back(new Platform(phys_world, 34.0f, 34.0f, 8.0f, 4.0f));
		levelobjects.push_back(new Platform(phys_world, 42.0f, 38.0f, 12.0f, -6.0f));
		levelobjects.push_back(new BouncingBall(phys_world, 10.0f, 10.0f));
*/
		levelobjects.push_back(new Platform(phys_world, 20.0, 40.0, 40.0, 0));
		levelobjects.push_back(new Platform(phys_world, 5.0, 20.0, 15.0, 20.0));
		levelobjects.push_back(new BigBall(phys_world, 6.0, 15.0));
		for (size_t i = 0; i<10; i++)
		{
			levelobjects.push_back(new Domino(phys_world, 25.0f+i*2.5f, 38.0f));
		}
		levelobjects.push_back(new BouncingBall(phys_world, 50.0f, 10.0f));
		level_loaded = true;
    }
    
     
    void draw(bool debug=false, bool drawsfml=true) {
      if (debug)
		phys_world.DrawDebugData();
      
      if (drawsfml) {
      	for (auto &iter : levelobjects) {
		iter->update_drawable();
		iter->draw(App);
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
    std::list<GameObject* > levelobjects;	//The level itself
    std::list<GameObject* > playerobjects;	//The objects that the player has placed
    
    
    
};
#endif //LEVEL_H
