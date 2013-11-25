#include "GameObject.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <SFML/System.hpp>
#include <cmath>


GameObject::~GameObject() {
	body_ptr->GetWorld()->DestroyBody(body_ptr);
	delete drawable;
}

b2Vec2 GameObject::getPos() const {
	return body_ptr->GetPosition();
}
std::string GameObject::getName(void) const {
	return name;
}
void GameObject::reset() {
	body_ptr->SetTransform(original_pos, original_rot);
	body_ptr->SetLinearVelocity(b2Vec2(0,0));
	body_ptr->SetAngularVelocity(0);
	body_ptr->SetAwake(true);
}


/*
Three nested for loops might look bad but with the small amount of shapes we have,
it should not be a problem. The algorithm's O notation speed is O(M*N), where
M=amount of shapes on the body.
N=amount of all other shapes.
*/
bool GameObject::noOverlaps() const {
	b2Body* body_list = body_ptr->GetWorld()->GetBodyList();
	for (;body_list!=NULL;body_list=body_list->GetNext()) {
		if (body_list==body_ptr) continue;

		b2Fixture* my_fixtures=body_ptr->GetFixtureList();
		b2Fixture* other_fixtures=body_list->GetFixtureList();
		for (;my_fixtures!=NULL;my_fixtures=my_fixtures->GetNext()) {
			for (;other_fixtures!=NULL;other_fixtures=other_fixtures->GetNext()) {
				if (b2TestOverlap(my_fixtures->GetShape(),0,other_fixtures->GetShape(),0,body_ptr->GetTransform(),body_list->GetTransform())) {
					return false;
				}
			}
		}
	}
	return true;
}


void GameObject::move(float x, float y) {

	body_ptr->SetTransform(b2Vec2(x,y) - local_mouse, body_ptr->GetAngle());
	if (noOverlaps()) {
		can_place=true;
	}
	else {
		can_place=false;
	}
	original_pos=b2Vec2(x,y) - local_mouse;
}

bool GameObject::isInside(float x, float y) {
	for (b2Fixture* f = body_ptr->GetFixtureList(); f!=NULL; f=f->GetNext()) {
		if (f->TestPoint(b2Vec2(x,y))) {
			local_mouse = body_ptr->GetLocalPoint(b2Vec2(x,y));
			return true;
		}
	}
	return false;
}

	






GameObject* GameObjectFactory(b2World& world, std::string name, float x, float y) {
	if (name=="Platform")
		return new Platform(world, x,y, 20.0f, 0.0f);
	if (name=="Wall")
		return new Wall(world, x,y, 0.0f, 20.0f);
	return NULL; //Name not found!
}


void GameObject::update_drawable() {
	drawable->update(body_ptr);
}
void GameObject::draw(sf::RenderWindow& win) {
	drawable->draw(win);
}
void GameObject::setHighlight(std::string type) {
	drawable->setHighlight(type,can_place);
}
bool GameObject::highlightPoint(sf::Vector2i point) {
	return (drawable->highlightPoint(point));
}
std::string GameObject::highlightClicked(sf::Vector2i point) {
	return drawable->highlightClicked(point);
}
void GameObject::highlightDelta(sf::Vector2i) {}

Domino::Domino(b2World& world, float x, float y) : GameObject(x,y,"Domino") {
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x, y);
    body_ptr = world.CreateBody(&bodyDef);
    b2PolygonShape dominoShape;
    dominoShape.SetAsBox(0.4, 1.5);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dominoShape;
    fixtureDef.density = 3.0f;
    fixtureDef.friction = 0.5f;
    body_ptr->CreateFixture(&fixtureDef);
}

Platform::Platform(b2World& world, float x, float y, float width, float heigth) : GameObject(x,y,"Platform", new PlatformDrawable(x,y,width,heigth)) { //BTW it should be height, not heigth.

	b2BodyDef bodyDef;
	bodyDef.position.Set(x, y);
	body_ptr = world.CreateBody(&bodyDef);
	b2Vec2 vertices[4];
	vertices[0].Set(0, 0);
	vertices[1].Set(width, heigth);
	vertices[2].Set(width, heigth + 2);
	vertices[3].Set(0, 2);
	b2PolygonShape polygonShape;
	polygonShape.Set(vertices, 4);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &polygonShape;
	body_ptr->CreateFixture(&fixtureDef);

	highlight_extras=true;
}
void Platform::highlightDelta(sf::Vector2i point) {
	sf::Vector2i delta = drawable->highlightDelta(point);
	b2Vec2 delta_convert((float)delta.x/10.0f,(float)delta.y/10.0f);
	b2PolygonShape* shape_ptr = dynamic_cast<b2PolygonShape*>(body_ptr->GetFixtureList()->GetShape());

	b2Vec2 vertices[4];
	for (int index=0;index<4;index++) {
		vertices[index]=shape_ptr->GetVertex(index);
	}
	vertices[0]+=delta_convert;
	vertices[1]+=delta_convert;
    
    //A small notch close to zero, you have to use a little more velocity:
    if (std::abs(vertices[1].y-vertices[2].y) < 0.2f) {
        vertices[0].y=0;
        vertices[1].y=2;
    }
    
	if (std::abs(vertices[1].y-vertices[2].y)/std::abs(vertices[1].x-vertices[2].x) < 1
		&& std::abs(vertices[1].x-vertices[2].x) > 3.0f && std::abs(vertices[1].x-vertices[2].x) < 40.0f) {
		shape_ptr->Set(vertices, 4);
	}
	if (!noOverlaps()) {
		vertices[0]-=delta_convert;
		vertices[1]-=delta_convert;
		shape_ptr->Set(vertices,4);
	}
}


Wall::Wall(b2World& world, float x, float y, float width, float heigth) : GameObject(x,y,"Wall", new PlatformDrawable(x,y,width,heigth)) {
	b2BodyDef bodyDef;
	bodyDef.position.Set(x, y);
	body_ptr = world.CreateBody(&bodyDef);
	b2Vec2 vertices[4];
	vertices[0].Set(0, 0);
	vertices[3].Set(2, 0);
	vertices[1].Set(width, heigth);
	vertices[2].Set(width +2, heigth);
	b2PolygonShape polygonShape;
	polygonShape.Set(vertices, 4);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &polygonShape;
	body_ptr->CreateFixture(&fixtureDef);		

	highlight_extras=true;
}
void Wall::highlightDelta(sf::Vector2i point) {
	sf::Vector2i delta = drawable->highlightDelta(point);
	b2Vec2 delta_convert((float)delta.x/10.0f,(float)delta.y/10.0f);
	b2PolygonShape* shape_ptr = dynamic_cast<b2PolygonShape*>(body_ptr->GetFixtureList()->GetShape());

	b2Vec2 vertices[4];
	for (int index=0;index<4;index++) {
		vertices[index]=shape_ptr->GetVertex(index);
	}
    
    //Because of the way B2D handles vertices, we have to check here which ones we want.
    size_t wanted_index;
    if (vertices[0].y < vertices[1].y) wanted_index=1;
    else wanted_index=0;
    
    
	vertices[wanted_index]+=delta_convert;
	vertices[wanted_index+1]+=delta_convert;
    
    //A small notch close to zero:
    if (std::abs(vertices[wanted_index+1].x-vertices[wanted_index+2].x) < 0.2f) {
        vertices[wanted_index].x=2;
        vertices[wanted_index+1].x=0;
    }
    
	if (std::abs(vertices[wanted_index+1].x-vertices[wanted_index+2].x)/std::abs(vertices[wanted_index+1].y-vertices[wanted_index+2].y) < 1
		&& std::abs(vertices[wanted_index +1].y-vertices[wanted_index +2].y) > 3.0f && std::abs(vertices[wanted_index +1].y-vertices[wanted_index +2].y) < 40.0f) {
		shape_ptr->Set(vertices, 4);
	}
    
	if (!noOverlaps()) {
		vertices[wanted_index]-=delta_convert;
		vertices[wanted_index+1]-=delta_convert;
		shape_ptr->Set(vertices,4);
	}
}

Ball::Ball(b2World& world, float x, float y, std::string name, float r, float restitution = 0, float density = 1.0) : GameObject(x,y,name) {
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(x, y);
	body_ptr = world.CreateBody(&bodyDef);	
	b2CircleShape circleShape;
	circleShape.m_p.Set(0, 0);
	circleShape.m_radius = r;
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape;
	fixtureDef.density = density;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = restitution;
	body_ptr->CreateFixture(&fixtureDef);
}

BouncingBall::BouncingBall(b2World& world, float x, float y) : Ball(world, x, y,"BouncingBall", 0.5, 0.5, 1.0) {}

BowlingBall::BowlingBall(b2World& world, float x, float y) : Ball(world, x, y,"BowlingBall", 0.8, 0.1, 3.0) {}

BigBall::BigBall(b2World& world, float x, float y) : Ball(world, x, y,"BigBall", 2.0, 0.1, 0.4) {}

/*
Chain::Chain(b2World& world) : GameObject(10,10) {
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(10, 10);
	body_ptr = world.CreateBody(&bodyDef);
	b2CircleShape circleShape;
	circleShape.m_p.Set(0, 0);
	circleShape.m_radius = 1;
b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape;
	fixtureDef.density = 1;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 0;
	body_ptr->CreateFixture(&fixtureDef);
	b2Body* first = body_ptr;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(10, 10);
	body_ptr = world.CreateBody(&bodyDef);
	b2PolygonShape chainShape;
	chainShape.SetAsBox(0.25, 0.1);
	
	fixtureDef.shape = &chainShape;
	fixtureDef.density = 0.1f;
	body_ptr->CreateFixture(&fixtureDef);
	b2RevoluteJointDef jointDef;
	jointDef.localAnchorA.Set(0.2, 0);
	jointDef.localAnchorB.Set(-0.2, 0);
	jointDef.bodyA = first;
	jointDef.bodyB = body_ptr;
	world.CreateJoint(&jointDef);
	b2Body* last = body_ptr;	
	for (int i=0; i<40; i++) {
		bodyDef.position.Set(10 + i*0.5, 10);
		b2Body* link = world.CreateBody(&bodyDef);
		link->CreateFixture(&fixtureDef);
		jointDef.bodyA = body_ptr;
		jointDef.bodyB = link;
		world.CreateJoint(&jointDef);
		body_ptr = link;
		last = link;
	}
	bodyDef.position.Set(10 + 20, 10);
	body_ptr = world.CreateBody(&bodyDef);
	circleShape.m_p.Set(0, 0);
	circleShape.m_radius = 1;
	fixtureDef.shape = &circleShape;
	fixtureDef.density = 1;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 0;
	body_ptr->CreateFixture(&fixtureDef);
	jointDef.bodyA = body_ptr;
	jointDef.bodyB = last;
	world.CreateJoint(&jointDef);
}
*/
