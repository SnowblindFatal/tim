#include "GameObject.h"
#include <SFML/Graphics.hpp>




void HorizontalBlock::update_drawable() {
	drawable.update(body_ptr);
}

void DroppingSquare::update_drawable() {
	drawable.update(body_ptr);
}
