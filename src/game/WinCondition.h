#ifndef WINCONDITION_H
#define WINCONDITION_H
#include <math.h>
#include <Box2D/Box2D.h>
#include "GameObject.h"
//An abstract class that represents the various win conditions
//Implent the pure virtual function check() in subclasses to create different conditions.
class WinCondition
{

public:
	WinCondition(GameObject* go_ptr) : go_ptr(go_ptr), fulfilled(false)  {}
	virtual ~WinCondition() {}
	
	virtual bool check() =0;
	virtual void reset() {
		fulfilled=false;
	}


protected:

	GameObject* go_ptr;
	bool fulfilled;
};

//A condition that checks wether the GameObject's center is within a square with side 2*tolerance, center (x,y)
class IsNearPoint : public WinCondition
{
public:
	IsNearPoint(GameObject* go_ptr, float x, float y, float tolerance = 3.0f) : WinCondition(go_ptr), x(x), y(y), tolerance(tolerance) {}
	
	bool check() {
		if ( (std::fabs(go_ptr->getPos().x-x) < tolerance) && (std::fabs(go_ptr->getPos().y-y) < tolerance) )
		{
			fulfilled=true;
			return fulfilled;
		}
		return fulfilled;
	}

private:
	float x;
	float y;
	float tolerance;
};

#endif
