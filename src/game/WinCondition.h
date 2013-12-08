#ifndef WINCONDITION_H
#define WINCONDITION_H
#include <cmath>
#include <Box2D/Box2D.h>
#include "GameObject.h"
#include <string>
//An abstract class that represents the various win conditions
//Implent the pure virtual function check() in subclasses to create different conditions.
class WinCondition
{

public:
	WinCondition(GameObject* go_ptr, std::string _name) : go_ptr(go_ptr), fulfilled(false), name(_name)  {}
	virtual ~WinCondition() {}
	
	virtual bool check() =0;
	virtual void draw(sf::RenderWindow&) =0;
	virtual bool highlightPoint(sf::Vector2i) {return false;}
	virtual void highlightDelta(sf::Vector2i) {}
	virtual std::string highlightClicked(sf::Vector2i) {return "nothing";}
	virtual void reset() {
		fulfilled=false;
	}
	// Used by FileHandler
	virtual GameObject* getObject() const
	{
		return go_ptr;
	}
	
	virtual std::string getName() const
	{
		return name;
	}


protected:

	GameObject* go_ptr;
	bool fulfilled;
	std::string name;
};

//A condition that checks wether the GameObject's center is within a square with side 2*tolerance, center (x,y)
class IsNearPoint : public WinCondition
{
public:
	IsNearPoint(GameObject* go_ptr, float x, float y, float tolerance = 3.0f) : WinCondition(go_ptr, "IsNearPoint"), x(x), y(y), tolerance(tolerance), local_mouse(0,0), move_active(false),resize_active(false) {}
	
	bool check() {
		if ( (std::fabs(go_ptr->getCurrentPos().x-x) < tolerance) && (std::fabs(go_ptr->getCurrentPos().y-y) < tolerance) )
		{
			fulfilled=true;
			return fulfilled;
		}
		return fulfilled;
	}
	
	b2Vec2 getPos()
	{
		return b2Vec2(x, y);
	}
	
	float getTolerance()
	{
		return tolerance;
	}
	void draw(sf::RenderWindow&);
	bool highlightPoint(sf::Vector2i); 
	void highlightDelta(sf::Vector2i);
	std::string highlightClicked(sf::Vector2i);

private:
	float x;
	float y;
	float tolerance;
	sf::Vector2i local_mouse;
	bool move_active;
	bool resize_active;
	PlatformHighlight highlight;		
};

class IsDestroyed : public WinCondition
{
	public:
	IsDestroyed(GameObject* go_ptr) : WinCondition(go_ptr, "IsDestroyed") {}
	
	bool check()
	{
		if (go_ptr->explodeStatus())
		{
			fulfilled = true;
			return fulfilled;
		}
		return fulfilled;
	}
	void draw(sf::RenderWindow&) {}
};

#endif
