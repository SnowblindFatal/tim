

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





class LevelData
{
public:
	LevelData(sf::RenderWindow& _App);
    ~LevelData();
    
    void addLevelObject(GameObject*);
    
    void addPlayerObject(GameObject*);
    
	void loadlevel();
    
    //Iterate over winconditions, if all return true, return true, else 
    bool checkWin() const;

	/*
	Create a new GameObject, if available. It is placed in playerobjects.
	A pointer to the created GameObject is also returned, so that it is possible for
	the object to follow the mouse until a suitable position is found.
	*/
	GameObject* createObject(std::string name, float x, float y);
	
	//Return the first found playerobject that the point is inside of.
	GameObject* isInsidePlayerObject(float x, float y) const;



    void draw(bool debug=false, bool drawsfml=true); //Cannot be const, because no DrawDebugData() const exists, for whatever reason


	//Reset the level to where it was before simulation.
   	void reset() ;
	
	//Delete and place in available.
	void deletePlayerObject(GameObject* obj);
	
    //One Box2D step:
    void simulate() ;
    
    bool loaded(void) const;
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
