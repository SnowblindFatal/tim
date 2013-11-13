#include "GameObject.h"
#include <SFML/Graphics.hpp>
#include <string>

GameObject* GameObjectFactory(b2World& world, std::string name, float x, float y) {
	if (name=="Platform")
		return new Platform(world, x,y, 20.0f, 0.0f);
	if (name=="Wall")
		return new Wall(world, x,y, 0.0f, 20.0f);
	return NULL; //Name not found!
}

void HorizontalBlock::update_drawable() {
	drawable.update(body_ptr);
}

void DroppingSquare::update_drawable() {
	drawable.update(body_ptr);
}
