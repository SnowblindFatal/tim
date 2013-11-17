#include "Drawable.h"

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
	
	sf::FloatRect polygon_bounds= polygon.getGlobalBounds();
	highlight.setSize(sf::Vector2f(polygon_bounds.width+10, polygon_bounds.height+10));
	highlight.setPosition(sf::Vector2f(polygon_bounds.left-5,polygon_bounds.top-5));
	highlight.setFillColor(sf::Color::Transparent);
} 

void PlatformDrawable::draw(sf::RenderWindow& win) {
	win.draw(polygon);
	win.draw(highlight);
	highlight.setFillColor(sf::Color::Transparent);
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
	sf::FloatRect polygon_bounds= polygon.getGlobalBounds();
	highlight.setSize(sf::Vector2f(polygon_bounds.width+10, polygon_bounds.height+10));
	highlight.setPosition(sf::Vector2f(polygon_bounds.left-5,polygon_bounds.top-5));
	
}

void PlatformDrawable::setHighlight(std::string type, bool can_place) {
	if (type=="active") {
		highlight.setFillColor(sf::Color(255,255,0,80)); //Yellow
	}
	if (type=="hover") {
		highlight.setFillColor(sf::Color(255,255,255,80)); //White
	}	
	if (type=="dragged" && can_place) {
		highlight.setFillColor(sf::Color(0,255,0,80)); //Green
	}
	if (type=="dragged" && !can_place) {
		highlight.setFillColor(sf::Color(255,0,0,80)); //Red
	}
}
