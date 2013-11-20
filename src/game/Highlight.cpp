#include "Highlight.h"
#include <string>
#include <iostream>

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



//PlatformHighlight:
PlatformHighlight::PlatformHighlight() : Highlight() {
	delta_height.setFillColor(sf::Color::Transparent);
	delta_height.setSize(sf::Vector2f(8.0f, 16.0f));
	delta_width.setFillColor(sf::Color::Transparent);
	delta_width.setSize(sf::Vector2f(16.0f, 8.0f));
}

void PlatformHighlight::set(std::string type, bool can_place) {
	if (type=="active") {
		rect.setFillColor(sf::Color(255,255,0,80)); //Yellow
		delta_height.setFillColor(sf::Color(0,0,255,150)); //Blue
		delta_width.setFillColor(sf::Color(0,0,255,150));
	}
	else Highlight::set(type, can_place);
}

void PlatformHighlight::update_rect(sf::FloatRect polygon_bounds) {
	delta_height.setPosition(sf::Vector2f(polygon_bounds.left+polygon_bounds.width+10,polygon_bounds.top - 10));
	delta_width.setPosition(sf::Vector2f(polygon_bounds.left+polygon_bounds.width+10,polygon_bounds.top+polygon_bounds.height + 2));
	Highlight::update_rect(polygon_bounds);
}
void PlatformHighlight::draw(sf::RenderWindow& win) {
	win.draw(delta_height);
	delta_height.setFillColor(sf::Color::Transparent);
	win.draw(delta_width);
	delta_width.setFillColor(sf::Color::Transparent);
	Highlight::draw(win);
}

bool PlatformHighlight::checkPoint(sf::Vector2i point) {	
	if (delta_height.getGlobalBounds().contains(point.x, point.y)) {
		local_mouse=point;
		width_active=false;
		height_active=true;
		return true;
	}
	else if (delta_width.getGlobalBounds().contains(point.x, point.y)) {
		local_mouse=point;
		width_active=true;
		height_active=false;
		return true;
	}
	else return false;
}

sf::Vector2i PlatformHighlight::getDelta(sf::Vector2i point) {
	sf::Vector2i result(0,0);

	if (height_active) {
		
		result = sf::Vector2i(0, point.y - local_mouse.y);
	}
	else if (width_active) {
		
		result = sf::Vector2i(point.x-local_mouse.x, 0);
	}

	local_mouse=point;
	return result;
}




