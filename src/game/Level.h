

#ifndef LEVEL_H
#define LEVEL_H
#include <list>
#include <utility>
#include "GameObject.h"
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "DebugDraw.h"
#include "WinCondition.h"
#include <map>
#include <string>


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

		//List the various GameObjects, there will of course be 0 available by default:
		available["Platform"] = 0;
		available["Wall"] = 0;
		available["BouncingBall"] = 0;
		available["BigBall"] = 0;
		available["BowlingBall"] = 0;
		available["Domino"] = 0;
		available["Chain"] = 0;

    }
    ~LevelData() {
        for (auto &iter : levelobjects) {
            delete iter;
        }
    }
	void loadlevel();
    
    bool checkWin() {
		for (auto it : winconditions) {
			if (!it->check())
				return false;
		}
		return true;
	}

	/*
	Create a new GameObject, if available. It is placed in playerobjects.
	A pointer to the created GameObject is also returned, so that it is possible for
	the object to follow the mouse until a suitable position is found.
	*/
	GameObject* createObject(std::string name, float x, float y) {
		if (available[name]==0) {
			return NULL; //Denoting failed creation.
		}
		else {
			available[name]--;
			playerobjects.push_back(GameObjectFactory(phys_world,name,x,y));
			return playerobjects.back();
		}
	}
	
	//Return the first found playerobject that the point is inside of.
	GameObject* isInsidePlayerObject(float x, float y) {
		for (auto it : playerobjects) {
			if (it->isInside(x,y))
				return it;
		}
		return NULL;
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

   	void reset() {
		for (auto it : levelobjects) {
			it->reset();
		}
		for (auto it : playerobjects) {
			it->reset();
		}
		for (auto it : winconditions) {
			it->reset();
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
   	std::list<WinCondition* > winconditions;
    std::map<std::string, size_t> available; //The objects available to be placed
    
};
#endif //LEVEL_H
