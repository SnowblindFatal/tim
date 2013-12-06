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
		available["BouncingBall"] = 0;
		available["BigBall"] = 0;
		available["BowlingBall"] = 0;
		available["Catapult"] = 0;
		available["Seesaw"] = 0;
		available["GravityChanger"] = 0;
		available["Bomb"] = 0;


}
LevelData::~LevelData() {
	
        for (auto& iter : levelobjects) {
            delete iter;
			iter=NULL;
        }
		for (auto& iter : playerobjects) {
            delete iter;
			iter=NULL;
        }
        for (auto& iter : winconditions) {
            delete iter;
			iter=NULL;
        }
}



void LevelData::addLevelObject(GameObject* obj) {
	levelobjects.push_back(obj);
}

bool LevelData::addPlayerObject(const std::string& name) {
	if (available.find(name) != available.end())
	{
		available[name]++;
		return true;
	}
	return false;
}

void LevelData::addWinCondition(WinCondition* cond) {
	winconditions.push_back(cond);
}

void LevelData::loadlevel() {

	std::cout << "load level\n";
	/*	
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
	*/
	//levelobjects.push_back(new Platform(phys_world, 15.0, 15.0, 100.0, 0));
	/*
	levelobjects.push_back(new BouncingBall(phys_world, 40, 0.0f));
	levelobjects.push_back(new BouncingBall(phys_world, 42, 0.0f));
	levelobjects.push_back(new BouncingBall(phys_world, 44, 0.0f));
	levelobjects.push_back(new BouncingBall(phys_world, 46, 0.0f));
	levelobjects.push_back(new BouncingBall(phys_world, 50, 0.0f));
	levelobjects.push_back(new BouncingBall(phys_world, 52, 0.0f));
	*/
	//levelobjects.push_back(new Lift(phys_world, 10, 30, 30, 30));
	//levelobjects.push_back(new Bomb (phys_world, 48.0f, 0.0f));
	//levelobjects.push_back(new Bomb (phys_world, 40.0f, 0.0f));
	//winconditions.push_back(new IsNearPoint(levelobjects.back(), 80.0f, 40.0f, 5.0f));
	//levelobjects.push_back(new Bomb (phys_world, 48.0f, 0.0f));
	//winconditions.push_back(new IsNearPoint(levelobjects.back(), 80.0f, 40.0f, 5.0f));
	//levelobjects.push_back(new GravityChanger(phys_world, 20.0f, 20.0f));

	available["Platform"]=5;
	available["Wall"]=1;
	available["BouncingBall"] = 4;
	available["Seesaw"] = 20;
	available["Bomb"] = 10;
	available["GravityChanger"] = 3;
	available["BowlingBall"] = 6;
	available["Domino"] = 20;
	available["BigBall"] = 10;
	level_loaded=true;
}

bool LevelData::checkWin() const {
		if (winconditions.begin()==winconditions.end()) return false;
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
			if (it->isInside(x,y)) {
                it->setMoveStartLocation(x, y);
                return it;
            }
		}
		return NULL;
}

void LevelData::draw(GameObject* priority, bool debug, bool drawsfml) {
      if (debug)
		phys_world.DrawDebugData();
      
      if (drawsfml) {
      	for (auto &iter : levelobjects) {
			iter->update_drawable();
			iter->draw(App);
      	}
      	for (auto &iter : playerobjects) {
			if (iter!=priority) {
				iter->update_drawable();
				iter->draw(App);
      		}
		}
		if (priority!=NULL) {
			priority->update_drawable();
			priority->draw(App);
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
		phys_world.SetGravity(default_gravity);
}



void LevelData::simulate() {
	phys_world.Step(timestep, velocityIterations, positionIterations);
}

b2World& LevelData::getWorld() 
{
	return phys_world;
}
    
bool LevelData::loaded(void) const {
	return level_loaded;
}

bool LevelData::hasGoals() const
{
	return !winconditions.empty();
}

void LevelData::deletePlayerObject(GameObject* obj) {
	//Basically, it should always be found. I dunno why I did this if:
	if (available.find(obj->getName())!=available.end()) {
		available[obj->getName()]++;
	}
	delete obj;
	playerobjects.remove(obj);
}
	
std::map<std::string, size_t>& LevelData::get_available() {
	return available;
}


	
	
	
	
	
	
	
	
	
	
	
	
	
