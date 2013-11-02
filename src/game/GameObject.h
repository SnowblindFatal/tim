

//AKU's temp solution for demo.
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <string>
#include "settings.h"
#include <Box2D/Box2D.h>
#include <SFML/graphics.hpp>
#include <iostream>

class GameObject
{

public:
    GameObject(std::string name, b2Body* body, sf::RectangleShape* sprite) : name(name), body_ptr(body), sprite_ptr(sprite) {}
	virtual void update_sprite() {
		b2Vec2 position = body_ptr->GetPosition();
        float32 angle = body_ptr->GetAngle();
		sprite_ptr->setPosition(sf::Vector2f(position.x * 10, position.y * 10));
        sprite_ptr->setRotation(angle);
        //std::cout << name << ":  " << position.x << " " << position.y << std::endl;;
	}
	sf::RectangleShape* sprite_ptr; //for the demo
protected:
	std::string name;
	b2Body* body_ptr;
	
};	




	
#endif //GAMEOBJECT_H

