#include "Highlight.h"
#include <string>
#include <iostream>

Highlight::Highlight() {
    rect.setFillColor(sf::Color::Transparent);
	del1.setFillColor(sf::Color::Transparent);
	del2.setFillColor(sf::Color::Transparent);
	del2.setSize(sf::Vector2f(13.0f,5.0f));
	del1.setSize(sf::Vector2f(13.0f,5.0f));
	delete_active=false;	
	rect.setPosition(sf::Vector2f(-400,-400));
	del1.setPosition(sf::Vector2f(-400,-400));
	del2.setPosition(sf::Vector2f(-400,-400));
	real=false;
	
}

void Highlight::set(const std::string& type, bool can_place) {
	if (!real) return;
	if (type=="active") {
		rect.setFillColor(sf::Color(255,255,0,80)); //Yellow
		del1.setFillColor(sf::Color(255,0,0,255)); //RED
		del2.setFillColor(sf::Color(255,0,0,255)); //YEAH
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

void Highlight::update_rect(const sf::FloatRect& polygon_bounds) {
    rect.setSize(sf::Vector2f(polygon_bounds.width+10, polygon_bounds.height+10));
	rect.setPosition(sf::Vector2f(polygon_bounds.left-5,polygon_bounds.top-5));
	del1.setRotation(0.0f);
	del2.setRotation(0.0f);
	del1.setOrigin(6.5f,2.5f);
	del2.setOrigin(6.5f,2.5f);
	del1.setPosition(sf::Vector2f(polygon_bounds.left-10,polygon_bounds.top-10));
	del2.setPosition(sf::Vector2f(polygon_bounds.left-10,polygon_bounds.top-10));
	del1.setRotation(45.0f);
	del2.setRotation(-45.0f);
	real=true;
	
}

void Highlight::draw(sf::RenderWindow& win) {
    win.draw(rect);
	win.draw(del1);
	win.draw(del2);
    rect.setFillColor(sf::Color::Transparent);
	del1.setFillColor(sf::Color::Transparent);
	del2.setFillColor(sf::Color::Transparent);
}

std::string Highlight::clicked(const sf::Vector2i& point) {
	if (!real) return "nothing";
	if (delete_active) {

		sf::FloatRect box=del1.getGlobalBounds();
		if (box.contains(point.x,point.y)) {
			return "delete";
		}
		else {
			delete_active=false;
			return "nothing";
		}
	}
	else return "nothing";
}
bool Highlight::checkPoint(sf::Vector2i point) {
	if (!real) return false;
	sf::FloatRect box=del1.getGlobalBounds();
	if (box.contains(point.x,point.y)) {
		delete_active=true;
		return true; 
	}
	else {
		delete_active=false;
		return false;
	}
}

//PlatformHighlight:
PlatformHighlight::PlatformHighlight() : Highlight() {
	delta_box.setFillColor(sf::Color::Transparent);
	delta_box.setSize(sf::Vector2f(8.0f, 16.0f));
}

void PlatformHighlight::set(const std::string& type, bool can_place) {
	if (type=="active") {
		delta_box.setFillColor(sf::Color(0,0,255,150)); //Blue
	}
	Highlight::set(type, can_place);
}

void PlatformHighlight::update_rect(const sf::FloatRect& polygon_bounds) {
	delta_box.setPosition(sf::Vector2f(polygon_bounds.left+polygon_bounds.width+10,polygon_bounds.top - 10));
	Highlight::update_rect(polygon_bounds);
}
void PlatformHighlight::draw(sf::RenderWindow& win) {
	win.draw(delta_box);
	delta_box.setFillColor(sf::Color::Transparent);
	Highlight::draw(win);
}

bool PlatformHighlight::checkPoint(sf::Vector2i point) {	
	if (delta_box.getGlobalBounds().contains(point.x, point.y)) {
		delete_active=false;
		return true;
	}
	else {
		sf::FloatRect box=del1.getGlobalBounds();
		if (box.contains(point.x,point.y)) {
			delete_active=true;
			return true; 
		}
	}
	return false;
}

GravityChangerHighlight::GravityChangerHighlight() : Highlight() {
	rotate.setFillColor(sf::Color::Transparent);
	rotate.setRadius(5);
}

void GravityChangerHighlight::set(const std::string& type, bool can_place) {
	if (type=="active") {
		rotate.setFillColor(sf::Color::Blue);
	}
	Highlight::set(type,can_place);
	return;
}

void GravityChangerHighlight::update_rect(const sf::FloatRect& rect) {
	rotate.setPosition(rect.left+rect.width+12, rect.top + 10);
	Highlight::update_rect(rect);
}

void GravityChangerHighlight::draw(sf::RenderWindow& win) {
	win.draw(rotate);
	rotate.setFillColor(sf::Color::Transparent);
	Highlight::draw(win);
}
bool GravityChangerHighlight::checkPoint(sf::Vector2i point) {	
	if (rotate.getGlobalBounds().contains(point.x, point.y)) {
		delete_active=false;
		rotate_active=true;
		return true;
	}
	else {
		sf::FloatRect box=del1.getGlobalBounds();
		if (box.contains(point.x,point.y)) {
			delete_active=true;
			rotate_active=false;
			return true; 
		}
	}
	return false;
}
std::string GravityChangerHighlight::clicked(const sf::Vector2i& point) {
	if (delete_active) {

		sf::FloatRect box=del1.getGlobalBounds();
		if (box.contains(point.x,point.y)) {
			return "delete";
		}
		else {
			delete_active=false;
			rotate_active=false;
			return "nothing";
		}
	}
	else if (rotate_active) {
		if (rotate.getGlobalBounds().contains(point.x, point.y)) {
			rotate_active=false;
			return "rotate";
		}
	}
	return "nothing";
}



