#include "Drawable.h"
#include "../general/Resources.h"
#include <string>

void Drawable::setHighlight(std::string type, bool can_place){
    highlight->set(type,can_place);
}
std::string Drawable::highlightClicked(sf::Vector2i point) {
	return highlight->clicked(point);
}
bool Drawable::highlightPoint(sf::Vector2i point) {
	return highlight->checkPoint(point);
}
sf::Vector2i Drawable::highlightDelta(sf::Vector2i point){
	return highlight->getDelta(point);
}

PlatformDrawable::PlatformDrawable(float x, float y, float width, float height) : Drawable(new PlatformHighlight) {
	x*=10;
	y*=10;
	width*=10;
	height*=10;
	polygon.setPointCount(4);
	polygon.setPoint(0, sf::Vector2f(x, y));
	polygon.setPoint(1, sf::Vector2f(x+width, y+height));
	polygon.setPoint(2, sf::Vector2f(x+width, y+height+20.0f));
	polygon.setPoint(3, sf::Vector2f(x, y+20.0f));
    
	polygon.setTexture(Resources::getInstance().getTexture("whitebrick.jpg"));
    
} 

void PlatformDrawable::draw(sf::RenderWindow& win) {
	win.draw(polygon);
    highlight->draw(win);
}

void PlatformDrawable::update(b2Body* ptr) {
	b2Vec2 pos = ptr->GetPosition();
	for (int index=0;index<4;index++) {
		b2Vec2 vert = dynamic_cast<b2PolygonShape*>(ptr->GetFixtureList()->GetShape())->GetVertex(index);
		polygon.setPoint(index, sf::Vector2f(pos.x*10 + vert.x*10, pos.y*10 + vert.y*10));
	}
    const sf::FloatRect rect = polygon.getGlobalBounds();
    polygon.setTextureRect(sf::IntRect(0, 0, rect.width, rect.height));
	highlight->update_rect(rect);
	
}
