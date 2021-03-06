#include "Drawable.h"
#include "../general/Resources.h"
#include <string>
#include <cmath>

void Drawable::setHighlight(std::string type, bool can_place){
    highlight->set(type,can_place);
}
std::string Drawable::highlightClicked(sf::Vector2i point) {
	return highlight->clicked(point);
}
bool Drawable::highlightPoint(sf::Vector2i point) {
	return highlight->checkPoint(point);
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
	polygon.setOutlineThickness(1);
	
	polygon.setOutlineColor(sf::Color(145,145,145));
    
} 

void PlatformDrawable::draw(sf::RenderWindow& win) {
	win.draw(polygon);
    highlight->draw(win);
}

void PlatformDrawable::update(const std::vector<PhysBody>& bodies) {
	b2Body* ptr= bodies[0].body_ptr;
	b2Vec2 pos = ptr->GetPosition();
	for (int index=0;index<4;index++) {
		b2Vec2 vert = dynamic_cast<b2PolygonShape*>(ptr->GetFixtureList()->GetShape())->GetVertex(index);
		polygon.setPoint(index, sf::Vector2f(pos.x*10 + vert.x*10, pos.y*10 + vert.y*10));
	}
    const sf::FloatRect rect = polygon.getGlobalBounds();
    polygon.setTextureRect(sf::IntRect(0, 0, rect.width, rect.height));
	highlight->update_rect(rect);	
}


CrateDrawable::CrateDrawable(float x, float y) : Drawable() {
	x*=10;
	y*=10;
	box.setSize(sf::Vector2f(30,30));
	box.setOrigin(15, 15);
	box.setPosition(x,y);
	box.setTexture(Resources::getInstance().getTexture("crate.jpg"));
	box.setOutlineThickness(1);
	box.setOutlineColor(sf::Color(205,133,63));
}

void CrateDrawable::draw(sf::RenderWindow& win) {
	win.draw(box);
	highlight->draw(win);
}

void CrateDrawable::update(const std::vector<PhysBody>& bodies) {
	box.setPosition(sf::Vector2f(bodies[0].body_ptr->GetPosition().x*10,bodies[0].body_ptr->GetPosition().y*10));
	box.setRotation(bodies[0].body_ptr->GetAngle()*180.0f/3.141592f);
	const sf::FloatRect rect=box.getGlobalBounds();
	highlight->update_rect(rect);
}


BombDrawable::BombDrawable(float x, float y) : Drawable() {
	x*=10;
	y*=10;
	exploded = false;
	explosion_status = 0;
	sf::Texture *texture = Resources::getInstance().getTexture("explosion.png");
	explosion.setTexture(*texture);	
	explosion.setPosition(x,y);
	explosion.setOrigin(64,64);
	box.setSize(sf::Vector2f(26,26));
	box.setOrigin(13, 13);
	box.setPosition(x,y);
	box.setTexture(Resources::getInstance().getTexture("small_tnt.jpg"));
	box.setOutlineThickness(1);
	box.setOutlineColor(sf::Color::Yellow);
}

void BombDrawable::draw(sf::RenderWindow& win) {
	if (exploded == false) {
		win.draw(box);
    	highlight->draw(win);
	}
	else {
		if (explosion_status != 40) {
			explosion.setTextureRect(sf::IntRect(explosion_status*128, 0, 128, 128));
			win.draw(explosion);
			explosion_status++;
		}
	}
}

void BombDrawable::update(const std::vector<PhysBody>& bodies) {
	box.setPosition(sf::Vector2f(bodies[0].body_ptr->GetPosition().x*10,bodies[0].body_ptr->GetPosition().y*10));
	box.setRotation(bodies[0].body_ptr->GetAngle()*180.0f/3.141592f);
	explosion.setPosition(sf::Vector2f(bodies[0].body_ptr->GetPosition().x*10,bodies[0].body_ptr->GetPosition().y*10));
	const sf::FloatRect rect=box.getGlobalBounds();
	highlight->update_rect(rect);
}

void BombDrawable::setExploded(bool param) {
	exploded = param;
}

void BombDrawable::setExplosionStatus(int num) {
	explosion_status = num;
}

GravityChangerDrawable::GravityChangerDrawable(float x, float y) : Drawable(new GravityChangerHighlight()) {
	x*=10;
	y*=10;
	box.setSize(sf::Vector2f(28,20));
	button.setSize(sf::Vector2f(10,10));
	box.setOrigin(14, 15);
	button.setOrigin(5,5);
	box.setPosition(x,y);
	button.setPosition(x,y-5);	
	box.setTexture(Resources::getInstance().getTexture("gravitybox.png"));
	button.setTexture(Resources::getInstance().getTexture("gravitybutton.png"));
	box.setOutlineThickness(1);
	box.setOutlineColor(sf::Color::White);
}
void GravityChangerDrawable::draw(sf::RenderWindow& win) {
	win.draw(button);
	win.draw(box);
	highlight->draw(win);
}

void GravityChangerDrawable::update(const std::vector<PhysBody>& bodies) {
	box.setPosition(sf::Vector2f(bodies[0].body_ptr->GetPosition().x*10,bodies[0].body_ptr->GetPosition().y*10));
	box.setRotation(bodies[0].body_ptr->GetAngle()*180.0f/3.141592f);
	button.setPosition(sf::Vector2f(bodies[1].body_ptr->GetPosition().x*10,bodies[1].body_ptr->GetPosition().y*10));
	const sf::FloatRect rect=box.getGlobalBounds();
	highlight->update_rect(rect);
}
	

BowlingBallDrawable::BowlingBallDrawable(float x, float y) : Drawable() {
	x*=10;
	y*=10;
	ball.setRadius(10);
	ball.setPosition(x,y);
	ball.setOrigin(10,10);
	ball.setTexture(Resources::getInstance().getTexture("bowlingball.png"));
	//ball.setOutlineThickness(3);
	//ball.setOutlineColor(sf::Color(0,0,128));
}

void BowlingBallDrawable::draw(sf::RenderWindow& win) {
	win.draw(ball);
	highlight->draw(win);
}

void BowlingBallDrawable::update(const std::vector<PhysBody>& bodies) {
	ball.setPosition(sf::Vector2f(bodies[0].body_ptr->GetPosition().x*10,bodies[0].body_ptr->GetPosition().y*10));
	ball.setRotation(bodies[0].body_ptr->GetAngle()*180.0f/3.141592f);
	const sf::FloatRect rect=ball.getGlobalBounds();
	highlight->update_rect(rect);
}

BouncingBallDrawable::BouncingBallDrawable(float x, float y) : Drawable() {
	x*=10;
	y*=10;
	ball.setRadius(5);
	ball.setPosition(x,y);
	ball.setOrigin(5,5);
	ball.setTexture(Resources::getInstance().getTexture("small_ball.png"));
	//ball.setOutlineThickness(3);
	//ball.setOutlineColor(sf::Color(0,0,128));
}

void BouncingBallDrawable::draw(sf::RenderWindow& win) {
	win.draw(ball);
	highlight->draw(win);
}

void BouncingBallDrawable::update(const std::vector<PhysBody>& bodies) {
	ball.setPosition(sf::Vector2f(bodies[0].body_ptr->GetPosition().x*10,bodies[0].body_ptr->GetPosition().y*10));
	ball.setRotation(bodies[0].body_ptr->GetAngle()*180.0f/3.141592f);
	const sf::FloatRect rect=ball.getGlobalBounds();
	highlight->update_rect(rect);
}

BigBallDrawable::BigBallDrawable(float x, float y) : Drawable() {
	x*=10;
	y*=10;
	ball.setRadius(20);
	ball.setPosition(x,y);
	ball.setOrigin(20,20);
	sf::Texture *tex = Resources::getInstance().getTexture("bigball.png");
	tex->setSmooth(true);
	ball.setTexture(tex);
	ball.setOutlineThickness(1);
	ball.setOutlineColor(sf::Color(0,0,128));
}

void BigBallDrawable::draw(sf::RenderWindow& win) {
	win.draw(ball);
	highlight->draw(win);
}

void BigBallDrawable::update(const std::vector<PhysBody>& bodies) {
	ball.setPosition(sf::Vector2f(bodies[0].body_ptr->GetPosition().x*10,bodies[0].body_ptr->GetPosition().y*10));
	ball.setRotation(bodies[0].body_ptr->GetAngle()*180.0f/3.141592f);
	const sf::FloatRect rect=ball.getGlobalBounds();
	highlight->update_rect(rect);
}




SeesawDrawable::SeesawDrawable(float x, float y) : Drawable(new SeesawHighlight()) {
	x*=10;
	y*=10;
	polygon.setPointCount(3);
	polygon.setPoint(0, sf::Vector2f(x, y));
	polygon.setPoint(1, sf::Vector2f(x+10, y+20));
	polygon.setPoint(2, sf::Vector2f(x-10, y+20));
	polygon.setTexture(Resources::getInstance().getTexture("block.jpg"));
	polygon.setOutlineThickness(1);	
	polygon.setOutlineColor(sf::Color(145,145,145));

	box.setSize(sf::Vector2f(100,4));
	box.setOrigin(50, 2);
	box.setPosition(x,y);
	box.setTexture(Resources::getInstance().getTexture("plank.jpg"));
	box.setOutlineThickness(1);
	box.setOutlineColor(sf::Color(218,165,11));
}

void SeesawDrawable::draw(sf::RenderWindow& win) {
	win.draw(box);
	win.draw(polygon);
    highlight->draw(win);
}

void SeesawDrawable::update(const std::vector<PhysBody>& bodies) {
	b2Body* ptr= bodies[0].body_ptr;
	b2Vec2 pos = ptr->GetPosition();
	for (int index=0;index<3;index++) {
		b2Vec2 vert = dynamic_cast<b2PolygonShape*>(ptr->GetFixtureList()->GetShape())->GetVertex(index);
		polygon.setPoint(index, sf::Vector2f(pos.x*10 + vert.x*10, pos.y*10 + vert.y*10));
	}
    //const sf::FloatRect rect = polygon.getGlobalBounds();
   // polygon.setTextureRect(sf::IntRect(0, 0, rect.width, rect.height));
	box.setPosition(sf::Vector2f(bodies[1].body_ptr->GetPosition().x*10,bodies[1].body_ptr->GetPosition().y*10));
	box.setRotation(bodies[1].body_ptr->GetAngle()*180.0f/3.141592f);
	const sf::FloatRect rect =box.getGlobalBounds();
	highlight->update_rect(rect);
}


CatapultDrawable::CatapultDrawable(float x, float y, bool flipped) : Drawable(new CatapultHighlight()), flipped(flipped) {
	x*=10;
	y*=10;
	polygon.setPointCount(3);
	polygon.setPoint(0, sf::Vector2f(x, y));
	polygon.setPoint(1, sf::Vector2f(x+10, y+20));
	polygon.setPoint(2, sf::Vector2f(x-10, y+20));
	polygon.setTexture(Resources::getInstance().getTexture("block.jpg"));
	polygon.setOutlineThickness(1);	
	polygon.setOutlineColor(sf::Color(145,145,145));

	box.setSize(sf::Vector2f(100,4));
	box.setOrigin(50, 2);
	box.setPosition(x,y);
	box.setFillColor(sf::Color(160,160,160));
	box.setOutlineThickness(1);
	box.setOutlineColor(sf::Color(128,128,128));
	if (!flipped) {
		box2.setSize(sf::Vector2f(4,20));
		box2.setOrigin(-46, 22);
		box2.setPosition(x,y);
		box2.setFillColor(sf::Color(160,160,160));
		box2.setOutlineThickness(1);
		box2.setOutlineColor(sf::Color(128,128,128));
	}
	else {
		box2.setSize(sf::Vector2f(4,20));
		box2.setOrigin(50, 22);
		box2.setPosition(x,y);
		box2.setFillColor(sf::Color(160,160,160));
		box2.setOutlineThickness(1);
		box2.setOutlineColor(sf::Color(128,128,128));
	}
}

void CatapultDrawable::setFlipped(bool flip) {
	flipped = flip;
}

void CatapultDrawable::draw(sf::RenderWindow& win) {
	win.draw(box);
	win.draw(box2);
	win.draw(polygon);
    highlight->draw(win);
}

void CatapultDrawable::update(const std::vector<PhysBody>& bodies) {
	b2Body* ptr= bodies[0].body_ptr;
	b2Vec2 pos = ptr->GetPosition();
	for (int index=0;index<3;index++) {
		b2Vec2 vert = dynamic_cast<b2PolygonShape*>(ptr->GetFixtureList()->GetShape())->GetVertex(index);
		polygon.setPoint(index, sf::Vector2f(pos.x*10 + vert.x*10, pos.y*10 + vert.y*10));
	}
    //const sf::FloatRect rect = polygon.getGlobalBounds();
   // polygon.setTextureRect(sf::IntRect(0, 0, rect.width, rect.height));
	box.setPosition(sf::Vector2f(bodies[1].body_ptr->GetPosition().x*10,bodies[1].body_ptr->GetPosition().y*10));
	box.setRotation(bodies[1].body_ptr->GetAngle()*180.0f/3.141592f);
	if (!flipped) {
		box2.setOrigin(-46, 22);
	}
	else {
		box2.setOrigin(50, 22);
	}
	box2.setPosition(sf::Vector2f(bodies[1].body_ptr->GetPosition().x*10,bodies[1].body_ptr->GetPosition().y*10));
	box2.setRotation(bodies[1].body_ptr->GetAngle()*180.0f/3.141592f);
	const sf::FloatRect rect =box.getGlobalBounds();
	highlight->update_rect(rect);
}

DominoDrawable::DominoDrawable(float x, float y) : Drawable() {
	x*=10;
	y*=10;
	box.setSize(sf::Vector2f(8,30));
	box.setOrigin(4, 15);
	box.setPosition(x,y);
	box.setFillColor(sf::Color(255,255,255));
	//box.setTexture(Resources::getInstance().getTexture("plank.jpg"));
	box.setOutlineThickness(1);
	box.setOutlineColor(sf::Color(145,145,145));
}

void DominoDrawable::draw(sf::RenderWindow& win) {
	win.draw(box);
    highlight->draw(win);
}

void DominoDrawable::update(const std::vector<PhysBody>& bodies) {
	box.setPosition(sf::Vector2f(bodies[0].body_ptr->GetPosition().x*10,bodies[0].body_ptr->GetPosition().y*10));
	box.setRotation(bodies[0].body_ptr->GetAngle()*180.0f/3.141592f);
	const sf::FloatRect rect=box.getGlobalBounds();
	highlight->update_rect(rect);
}

TeleportDrawable::TeleportDrawable(float x, float y) : Drawable(new GravityChangerHighlight()) {
	x*=10;
	y*=10;
	box.setSize(sf::Vector2f(100,20));
	box.setOrigin(50,10);
	box.setPosition(x,y);
	box.setTexture(Resources::getInstance().getTexture("teleport.png"));
	box.setOutlineThickness(1);
	box.setOutlineColor(sf::Color::White);
}

void TeleportDrawable::draw(sf::RenderWindow& win) {
	win.draw(box);
	highlight->draw(win);
}

void TeleportDrawable::update(const std::vector<PhysBody>& bodies) {
	if (std::fabs(bodies[0].original_rot) <0.01f) {		
		box.setPosition(sf::Vector2f(bodies[0].body_ptr->GetPosition().x*10,bodies[0].body_ptr->GetPosition().y*10-8));
	}
	else if (std::fabs(bodies[0].original_rot) <3.143/2.0f) {		
		box.setPosition(sf::Vector2f(bodies[0].body_ptr->GetPosition().x*10+8,bodies[0].body_ptr->GetPosition().y*10));
	}
	else if (std::fabs(bodies[0].original_rot) <3.143f) {		
		box.setPosition(sf::Vector2f(bodies[0].body_ptr->GetPosition().x*10,bodies[0].body_ptr->GetPosition().y*10+8));
	}
	else if (std::fabs(bodies[0].original_rot) <3.0f*3.143/2.0f) {		
		box.setPosition(sf::Vector2f(bodies[0].body_ptr->GetPosition().x*10-8,bodies[0].body_ptr->GetPosition().y*10));
	}
	box.setRotation(bodies[0].body_ptr->GetAngle()*180.0f/3.141592f);
	const sf::FloatRect rect=box.getGlobalBounds();
	highlight->update_rect(rect);
}



