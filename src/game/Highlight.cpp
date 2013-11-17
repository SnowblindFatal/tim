#include "Highlight.h"

Highlight::Highlight() {
    rect.setFillColor(sf::Color::Transparent);
}

void Highlight::set(std::string type, bool can_place) {
	if (type=="active") {
		rect.setFillColor(sf::Color(255,255,0,80)); //Yellow
	}
	if (type=="hover") {
		rect.setFillColor(sf::Color(255,255,255,80)); //White
	}
	if (type=="dragged" && can_place) {
		rect.setFillColor(sf::Color(0,255,0,80)); //Green
	}
	if (type=="dragged" && !can_place) {
		rect.setFillColor(sf::Color(255,0,0,80)); //Red
	}
}

void Highlight::update_rect(sf::FloatRect polygon_bounds) {
    rect.setSize(sf::Vector2f(polygon_bounds.width+10, polygon_bounds.height+10));
	rect.setPosition(sf::Vector2f(polygon_bounds.left-5,polygon_bounds.top-5));
}

void Highlight::draw(sf::RenderWindow& win) {
    win.draw(rect);
    rect.setFillColor(sf::Color::Transparent);
}


