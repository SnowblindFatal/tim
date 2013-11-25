#include "Level.h"

namespace {
    b2Vec2 default_gravity(0.0f, 9.8f);
    float32 timestep = 1.0f/60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;
}

LevelData::LevelData(sf::RenderWindow& _App) : phys_world(default_gravity), App(_App), DebugDrawInstance(_App), level_loaded(false)
	{
        
		phys_world.SetContactListener(&collisions);
		phys_world.SetDebugDraw(&DebugDrawInstance);
        DebugDrawInstance.SetFlags(b2Draw::e_shapeBit);
		DebugDrawInstance.AppendFlags(b2Draw::e_centerOfMassBit);

		//List the various GameObjects, there will of course be 0 available by default:
		available["Platform"] = 0;
		available["Wall"] = 0;
		//available["BouncingBall"] = 0;
		available["BigBall"] = 0;
		available["BowlingBall"] = 0;
		available["Domino"] = 0;
		available["Chain"] = 0;

}
LevelData::~LevelData() {
        for (auto iter : levelobjects) {
            delete iter;
        }
		for (auto iter : playerobjects) {
            delete iter;
        }
        for (auto iter : winconditions) {
            delete iter;
        }
}



void LevelData::addLevelObject(GameObject* obj) {
	levelobjects.push_back(obj);
}

void LevelData::addPlayerObject(GameObject* obj) {
	playerobjects.push_back(obj);	
}

void LevelData::loadlevel() {

	std::cout << "load level\n";
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

	levelobjects.push_back(new Platform(phys_world, 20.0, 40.0, 40.0, 0));
	levelobjects.push_back(new Platform(phys_world, 5.0, 20.0, 15.0, 20.0));
	levelobjects.push_back(new BigBall(phys_world, 6.0, 15.0));
	
	levelobjects.push_back(new BouncingBall(phys_world, 50.0f, 10.0f));
	levelobjects.push_back(new Chain(phys_world));

	levelobjects.push_back(new Platform(phys_world, 15.0, 12.0, 1.0, 0));
	levelobjects.push_back(new Platform(phys_world, 35.0, 12.0, 1.0, 0));
	level_loaded = true;
	*/


	//Load a "bare minimum" playable level:
	//levelobjects.push_back(new Bomb(phys_world, 30, 35));

	/*	
	for (size_t i = 0; i<10; i++)
	{
		levelobjects.push_back(new Domino(phys_world, 25.0f+i*2.5f, 38.0f));
	}
*/
	/* Chain test
	levelobjects.push_back(new Chain(phys_world));
	levelobjects.push_back(new Platform(phys_world, 15.0, 15.0, 2.0, 0));
	levelobjects.push_back(new Platform(phys_world, 25.0, 15.0, 2.0, 0));
	*/
	
	for (size_t i = 0; i<7; i++)
	{
		for (size_t j=0; j<4; j++)
		{		
			if (i%2 != 1)
			{
				levelobjects.push_back(new Seesaw(phys_world, 5+j*19, 10+i*4));
			}
			else
			{
				if (j==3)
					break;
				levelobjects.push_back(new Seesaw(phys_world, 14.5+j*19, 10+i*4));
			}
		}
	}
	//levelobjects.push_back(new Platform(phys_world, 15.0, 15.0, 100.0, 0));
	levelobjects.push_back(new BouncingBall(phys_world, 40, 0.0f));
	levelobjects.push_back(new BouncingBall(phys_world, 42, 0.0f));
	levelobjects.push_back(new BouncingBall(phys_world, 44, 0.0f));
	levelobjects.push_back(new BouncingBall(phys_world, 46, 0.0f));
	levelobjects.push_back(new BouncingBall(phys_world, 50, 0.0f));
	levelobjects.push_back(new BouncingBall(phys_world, 52, 0.0f));
	levelobjects.push_back(new Bomb (phys_world, 48.0f, 0.0f));
	winconditions.push_back(new IsNearPoint(levelobjects.back(), 80.0f, 40.0f, 5.0f));
	available["Platform"]=5;
	available["Wall"]=1;
	available["BouncingBall"] = 4;
	available["Seesaw"] = 2;
}

bool LevelData::checkWin() const {
		for (auto it : winconditions) {
			if (!it->check())
				return false;
		}
		return true;
}
	
	
GameObject* LevelData::createObject(std::string name, float x, float y) {
		if (available[name]==0) {
			return NULL; //Denoting failed creation.
		}
		else {
			available[name]--;
			playerobjects.push_back(GameObjectFactory(phys_world,name,x,y));
			return playerobjects.back();
		}
}

GameObject* LevelData::isInsidePlayerObject(float x, float y) const {
		for (auto it : playerobjects) {
			if (it->isInside(x,y))
				return it;
		}
		return NULL;
}

void LevelData::draw(bool debug, bool drawsfml) {
      if (debug)
		phys_world.DrawDebugData();
      
      if (drawsfml) {
      	for (auto &iter : levelobjects) {
			iter->update_drawable();
			iter->draw(App);
      	}
      	for (auto &iter : playerobjects) {
			iter->update_drawable();
			iter->draw(App);
      	}
	}
}

void LevelData::reset() {
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



void LevelData::simulate() {
	phys_world.Step(timestep, velocityIterations, positionIterations);
	if (levelobjects.back()->contactStatus() == true && levelobjects.back()->explodeStatus() == false) {
		std::cout << "EXPLODE!!!\n";
		levelobjects.back()->explode();
	}
}
    
bool LevelData::loaded(void) const {
	return level_loaded;
}

void LevelData::deletePlayerObject(GameObject* obj) {
	//Basically, it should always be found. I dunno why I did this if:
	if (available.find(obj->getName())!=available.end()) {
		available[obj->getName()]++;
	}
	delete obj;
	playerobjects.remove(obj);
}
	



	
	
	
	
	
	
	
	
	
	
	
	
	
