#include "GameObject.h"
#include <SFML/Graphics.hpp>




void HorizontalBlock::update_drawable() {
	drawable.update(body_ptr->GetPosition().x, body_ptr->GetPosition().y, body_ptr->GetAngle());
}

void DroppingSquare::update_drawable() {
	drawable.update(body_ptr->GetPosition().x, body_ptr->GetPosition().y, body_ptr->GetAngle());
}
