#include "Drawable.h"

void Drawable::setHighlight(std::string type, bool can_place){
    highlight.set(type,can_place);
}

PlatformDrawable::PlatformDrawable(float x, float y, float width, float height) : highlighted(false) {
	x*=10;
	y*=10;
	width*=10;
	height*=10;
	polygon.setPointCount(4);
	polygon.setPoint(0, sf::Vector2f(x, y));
	polygon.setPoint(1, sf::Vector2f(x+width, y+height));
	polygon.setPoint(2, sf::Vector2f(x+width, y+height+20.0f));
	polygon.setPoint(3, sf::Vector2f(x, y+20.0f));
	polygon.setFillColor(sf::Color::White); //This should be replaced with a texture.
    
} 

void PlatformDrawable::draw(sf::RenderWindow& win) {
	win.draw(polygon);
    highlight.draw(win);
}

void PlatformDrawable::update(b2Body* ptr) {
	b2Vec2 pos = ptr->GetPosition();
	for (int index=0;index<4;index++) {
		b2Vec2 vert = dynamic_cast<b2PolygonShape*>(ptr->GetFixtureList()->GetShape())->GetVertex(index);
		if (index < 1) {
			polygon.setPoint(index, sf::Vector2f(pos.x*10 + vert.x*10, pos.y*10 + vert.y*10));
		}
		else {
			polygon.setPoint(index, sf::Vector2f(pos.x*10 + vert.x*10, pos.y*10 + vert.y*10 ));
		}
	}
	highlight.update_rect(polygon.getGlobalBounds());
	
}
