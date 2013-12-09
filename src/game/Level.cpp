#include "Level.h"
#include "FileHandler.h"
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
	available["Wall"] =0;
	available["BouncingBall"] = 0;
	available["BigBall"] = 0;
	available["BowlingBall"] = 0;
	available["Catapult"] = 0;
	available["Seesaw"] = 0;
	available["GravityChanger"] = 0;
	available["Bomb"] = 0;
	available["Domino"] = 0;
	
	description = "No description available.";
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

/*
void LevelData::loadlevel() {

	std::cout << "load level\n";

	FileHandler fh("res/leveldata/level1.txt");
	if (fh.loadLevel(*this)) {
		level_loaded=true;
	} else {
		std::cout << "error: " << fh.getError() << std::endl;
		// Return to level selection screen
	}

	available["Platform"]=5;
	available["Wall"]=1;
	available["BouncingBall"] = 4;
	available["Seesaw"] = 20;
	available["Bomb"] = 100;
	available["GravityChanger"] = 3;
	available["BowlingBall"] = 6;
	available["Domino"] = 20;
	available["BigBall"] = 10;
	
	
}
*/

void LevelData::setLoaded(bool b)
{
	level_loaded = b;
}

bool LevelData::checkWin() const {
		if (winconditions.begin()==winconditions.end()) return false;
		for (auto it : winconditions) {
			if (!it->check())
				return false;
		}
		return true;
}
	
	
GameObject* LevelData::createObject(std::string name, float x, float y, bool edit) {
		if (edit) {
			levelobjects.push_back(GameObjectFactory(phys_world,name,x,y));
			return levelobjects.back();
		}
		
		if (available[name]==0) {
			return NULL; //Denoting failed creation.
		}
		else {
			available[name]--;
			playerobjects.push_back(GameObjectFactory(phys_world,name,x,y));
			return playerobjects.back();
		}
}

GameObject* LevelData::isInsideLevelObject(float x, float y) const {
		for (auto it : levelobjects) {
			if (it->isInside(x,y)) {
                it->setMoveStartLocation(x, y);
                return it;
            }
		}
		return NULL;
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
			if (iter!=priority) {
				iter->update_drawable();
				iter->draw(App);
			}
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

void LevelData::drawWincondition(GameObject* target) {
	for (auto ptr : winconditions) {
		if (ptr->getObject() == target) {
			ptr->draw(App);
		}
	}
}

bool LevelData::winconditionPoint(sf::Vector2i mouse, GameObject* target) {
	for (auto ptr : winconditions) {
		if (ptr->getObject() == target) {
			return ptr->highlightPoint(mouse);
		}
	}
	return false;
}
void LevelData::winconditionDelta(sf::Vector2i mouse, GameObject* target) {
	for (auto ptr : winconditions) {
		if (ptr->getObject() == target) {
			return ptr->highlightDelta(mouse);
		}
	}
}
std::string LevelData::winconditionClicked(sf::Vector2i mouse, GameObject* target) {
	for (auto ptr : winconditions) {
		if (ptr->getObject() == target) {
			return ptr->highlightClicked(mouse);
		}
	}
	return "nothing";
}

void LevelData::deleteWincondition(GameObject* target) {
	for (auto ptr : winconditions) {
		if (ptr->getObject() == target) {
			delete ptr;
			winconditions.remove(ptr);
			return;
		}
	}
}

bool LevelData::canCreateWincondition(GameObject* target) {
	if (target==NULL) {
		return false;
	}
	for (auto ptr : winconditions) {
		if (ptr->getObject() == target) {
			return false;
		}
	}
	return true;
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

void LevelData::clear()
{
	auto levelit = levelobjects.begin();
	auto playerit = playerobjects.begin();
	auto winit = winconditions.begin();
	
	while (!levelobjects.empty())
	{
		levelit = levelobjects.begin();
		delete *levelit;
		levelit = levelobjects.erase(levelit);
	}
	while (!playerobjects.empty())
	{
		playerit = playerobjects.begin();
		delete *playerit;
		playerit = playerobjects.erase(playerit);
	}
	while (!winconditions.empty())
	{
		winit = winconditions.begin();
		delete *winit;
		winit = winconditions.erase(winit);
	}
	
	for (auto iter = available.begin();iter != available.end();iter++)
		iter->second = 0;
	
	description = "No description available.";
	level_loaded = false;
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

void LevelData::deleteLevelObject(GameObject* obj) {
	delete obj;
	levelobjects.remove(obj);
}
	
	
std::map<std::string, size_t>& LevelData::get_available() {
	return available;
}

const std::list<GameObject*>& LevelData::getLevelObjects() const 
{
	return levelobjects;
}

const std::list<WinCondition*>& LevelData::getWinConditions() const
{
	return winconditions;
}

const std::string LevelData::getDescription() const
{
	return description;
}

void LevelData::setDescription(std::string str)
{
	description = str;
}
