

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
#include "CollisionDetection.h" 
#include <map>
#include <string>
#include <iostream>





class LevelData
{
public:
	LevelData(sf::RenderWindow& _App);
    ~LevelData();
    
    void addLevelObject(GameObject*);
    
    bool addPlayerObject(const std::string&);
    
    void addWinCondition(WinCondition*);
    
	void setLoaded(bool);
    
    //Iterate over winconditions, if all return true, return true, else false
    bool checkWin() const;

	/*
	Create a new GameObject, if available. It is placed in playerobjects.
	A pointer to the created GameObject is also returned, so that it is possible for
	the object to follow the mouse until a suitable position is found.
	The edit flag is for EditMode, where we don't need to check if available.
	*/
	GameObject* createObject(std::string name, float x, float y, bool edit=false);
	
	//Return the first found playerobject/levelobject that the point is inside of.
	GameObject* isInsidePlayerObject(float x, float y) const;
	GameObject* isInsideLevelObject(float x, float y) const;


    void draw(GameObject* priority, bool debug=false, bool drawsfml=true); //Cannot be const, because no DrawDebugData() const exists, for whatever reason
	void drawWincondition(GameObject*); //Used in EditMode
	bool winconditionPoint(sf::Vector2i, GameObject*);
	void winconditionDelta(sf::Vector2i, GameObject*);
	std::string winconditionClicked(sf::Vector2i, GameObject*);
	void deleteWincondition(GameObject*);
	bool canCreateWincondition(GameObject*);



	//Reset the level to where it was before simulation.
   	void reset() ;
   	
   	void clear(); // Empty level of all objects, win conditions and the description
	
	//Delete and place in available.
	void deletePlayerObject(GameObject* obj);
	void deleteLevelObject(GameObject* obj);
	
    //One Box2D step:
    void simulate() ;
    
    //Used by FileHandler
    b2World& getWorld();
    
    bool loaded(void) const;
    
    //Used by FileHandler
    bool hasGoals() const;
	
	//This for access
	std::map<std::string, size_t>& get_available();
	
	//Used by FileHandler
	const std::list<GameObject*>& getLevelObjects() const;
	const std::list<WinCondition*>& getWinConditions() const;
	
	const std::string getDescription() const;
	void setDescription(std::string);
	
 private:
    
    b2World phys_world;
    sf::RenderWindow& App;
    DebugDraw DebugDrawInstance;
    CollisionDetection collisions;
	
    bool level_loaded;
    std::string description; // Instructions on how to complete the level
    std::list<GameObject* > levelobjects;	//The level itself
    std::list<GameObject* > playerobjects;	//The objects that the player has placed
   	std::list<WinCondition* > winconditions;
    std::map<std::string, size_t> available; //The objects available to be placed
    
};



#endif //LEVEL_H
