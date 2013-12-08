#include "WinCondition.h"
#include "GameObject.h"

void IsNearPoint::draw(sf::RenderWindow& win) {
	highlight.update_rect(sf::FloatRect(((float)x-tolerance)*10.0f,((float)y-tolerance)*10.0f,tolerance*20.0f,tolerance*20.0f));
	highlight.set("wincondition", true);
	highlight.draw(win);
}

bool IsNearPoint::highlightPoint(sf::Vector2i mouse) {
	sf::FloatRect bounds(((float)x-tolerance)*10.0f,((float)y-tolerance)*10.0f,tolerance*20.0f,tolerance*20.0f);
	local_mouse=mouse;
	if (bounds.contains(mouse.x, mouse.y)) {
		move_active=true;
		resize_active=false;
		return true;
	}
	else if (highlight.checkPoint(mouse)) {
		resize_active=highlight.delete_active?false:true;
		move_active=false;
		return true;
	}
	else return false;
}

void IsNearPoint::highlightDelta(sf::Vector2i mouse) {
	if (move_active) {
		x+=(mouse.x-local_mouse.x)/10.0f;
		y+=(mouse.y-local_mouse.y)/10.0f;
	}
	if (resize_active) {
		tolerance+=(mouse.x-local_mouse.x)/20.0f;
		if (tolerance<0.5f) {	
			tolerance=0.5f;
		}
	}
	local_mouse=mouse;
}
std::string IsNearPoint::highlightClicked(sf::Vector2i mouse) {
	move_active=false;
	resize_active=false;
	return highlight.clicked(mouse);
}


void IsDestroyed::draw(sf::RenderWindow& win) {
	b2Vec2 pos=go_ptr->getCurrentPos();
	highlight.update_rect(sf::FloatRect(pos.x*10-30,pos.y*10-30,60,60));
	highlight.set("wincondition", true);
	highlight.draw(win);
}
bool IsDestroyed::highlightPoint(sf::Vector2i mouse) {
	if (highlight.checkPoint(mouse)) {
		return true;
	}
	else return false;
}
std::string IsDestroyed::highlightClicked(sf::Vector2i mouse) {
	return highlight.clicked(mouse);
}
