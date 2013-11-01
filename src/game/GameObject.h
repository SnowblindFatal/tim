

//AKU's temp solution for demo.
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <string>
#include "settings.h"
#include <Box2D.h>
#include <SFML/graphics.hpp>


class GameObject
{

public:
	virtual GameObject(std::string name, b2Body* body, sf::Sprite* sprite) : name(name), body_ptr(body), sprite_ptr(sprite) {}
	virtual void update_sprite() {
		b2Vec2 position = body_ptr->GetPosition();
		sprite_ptr->setPosition(sf::Vector2f(position.x * 10, position.y * 10));
	}
	
protected:
	std::string name;
	b2Body* body_ptr;
	sf::Sprite* sprite_ptr;
};	




	
#endif //GAMEOBJECT_H

