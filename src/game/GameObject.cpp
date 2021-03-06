#include "GameObject.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <SFML/System.hpp>
#include <cmath>
#include <iostream>
#include "PhysBody.h"
#include <algorithm>


GameObject::~GameObject() {
	for (auto& body : bodies) {
		body.body_ptr->GetWorld()->DestroyBody(body.body_ptr);
		body.body_ptr=NULL;
	}
	delete drawable;
	drawable=NULL;
}

b2Vec2 GameObject::getPos() const {
	return position;
}

b2Vec2 GameObject::getCurrentPos() const
{
	// Calculate current body position as a mean value of all shapes
	float x = 0;
	float y = 0;
	for (auto& body : bodies)
	{
		x += body.body_ptr->GetPosition().x;
		y += body.body_ptr->GetPosition().y;
	}
	return b2Vec2(x/bodies.size(),y/bodies.size()); // ### WARNING! DIVISION BY ZERO IF GAMEOBJECT HAS NO BODIES! ###
}

std::string GameObject::getName(void) const {
	return name;
}

int GameObject::getID() const
{
	return id;
}

void GameObject::setID(int i)
{
	id = i;
}

void GameObject::reset() {
	for (auto& body : bodies) {
		body.body_ptr->SetTransform(body.original_pos, body.original_rot);
		body.body_ptr->SetLinearVelocity(b2Vec2(0,0));
		body.body_ptr->SetAngularVelocity(0);
		body.body_ptr->SetAwake(true);
	}
}


/*
Three nested for loops might look bad but with the small amount of shapes we have,
it should not be a problem. The algorithm's O notation speed is O(M*N), where
M=amount of shapes on the body.
N=amount of all other shapes.
*/
bool GameObject::noOverlaps() const {
	b2Body* body_list = bodies[0].body_ptr->GetWorld()->GetBodyList();
	for (;body_list!=NULL;body_list=body_list->GetNext()) {
		bool on_the_same=false;
		bool found=false;
		for (auto& body : bodies) {
			if (body_list==body.body_ptr) {
				on_the_same=true;
				break;
			}
			if (found) continue;

			b2Fixture* my_fixtures=body.body_ptr->GetFixtureList();
			b2Fixture* other_fixtures=body_list->GetFixtureList();
			for (;my_fixtures!=NULL;my_fixtures=my_fixtures->GetNext()) {
				for (;other_fixtures!=NULL;other_fixtures=other_fixtures->GetNext()) {
					if (b2TestOverlap(my_fixtures->GetShape(),0,other_fixtures->GetShape(),0,body.body_ptr->GetTransform(),body_list->GetTransform())) {
						found=true;
						break;
					}
				}
				if (found) break;
				other_fixtures=body_list->GetFixtureList();
			}
		}
		if (found && !on_the_same) return false;
	}
	return true;
}

void GameObject::moveDiscretely(float x, float y) 
{
//    std::cout << "originals: x: " << manipulationStartLocation.x << ", y: " << manipulationStartLocation.y << "\n";
//    std::cout << "new: x: " << x << ", y: " << y << "\n";
//    std::cout << "x: " << std::fmod((x - manipulationStartLocation.x), 1.0f) << ", y: " << std::fmod((y - manipulationStartLocation.y), 1.0f) << "\n";
    x -= std::fmod((x - moveStartLocation.x), DISCRETE_MOVE_UNIT);
    y -= std::fmod((y - moveStartLocation.y), DISCRETE_MOVE_UNIT);
    GameObject::move(x, y);
}
void GameObject::setMoveStartLocation(float x, float y)
{
    moveStartLocation = b2Vec2(x, y);
}

void GameObject::setManipulationStartLocation(sf::Vector2i pos) {
    manipulationReferenceLocation = b2Vec2(pos.x / 10.0f, pos.y / 10.0f);
}
void GameObject::move(float x, float y) {
    b2Vec2 mousePos(x, y);
	for (auto& body : bodies) {
		body.body_ptr->SetTransform(body.body_ptr->GetTransform().p+ (mousePos - local_mouse), body.body_ptr->GetAngle());
		body.original_pos=body.body_ptr->GetTransform().p;
	}
    position += mousePos - local_mouse;
	local_mouse = mousePos;
	if (noOverlaps()) {
		can_place=true;
	}
	else {
		can_place=false;
	}
}

void GameObject::teleport(b2Vec2 target, b2Vec2 velocity, bool flip) {
	teleporttarget=target;	
	teleportSet=true;
	teleportvelocity=velocity;
	teleportflip=flip;
}

b2Vec2 GameObject::getLinearVelocity() {
	return bodies[0].body_ptr->GetLinearVelocity();
}

bool GameObject::isInside(float x, float y) {
	local_mouse = b2Vec2(x,y);
	for (auto& body : bodies) {
		for (b2Fixture* f = body.body_ptr->GetFixtureList(); f!=NULL; f=f->GetNext()) {
			if (f->TestPoint(b2Vec2(x,y))) {
				return true;
			}
		}
	}
	return false;
}


GameObject* GameObjectFactory(b2World& world, std::string name, float x, float y) {
	if (name=="Platform")
		return new Platform(world, x,y, 20.0f, 0.0f);
	if (name=="Wall")
		return new Wall(world, x,y, 0.0f, 20.0f);
	if (name=="BouncingBall")
		return new BouncingBall(world, x, y);
	if (name=="BowlingBall")
		return new BowlingBall(world, x, y);
	if (name=="BigBall")
		return new BigBall(world, x, y);
	if (name=="Seesaw")
		return new Seesaw(world, x, y);
	if (name=="Bomb")
		return new Bomb(world, x, y);
	if (name=="GravityChanger")
		return new GravityChanger(world,x,y);
	if (name=="Catapult")
		return new Catapult(world,x,y);
	if (name=="Domino")
		return new Domino(world,x,y);
	if (name=="Teleport")
		return new Teleport(world,x,y);
	if (name=="Crate")
		return new Crate(world,x,y);
	return NULL; //Name not found!
}


void GameObject::update_drawable() {
	drawable->update(bodies);
	if (teleportReady==true && (name=="BigBall" || name=="BouncingBall" || name=="BowlingBall" || name=="Domino" || name=="Crate")) {
		if (teleportflip==false) {
			bodies[0].body_ptr->SetTransform(teleporttarget, bodies[0].body_ptr->GetAngle());
		}
		else {
			bodies[0].body_ptr->SetTransform(teleporttarget, bodies[0].body_ptr->GetAngle()+3.141592f/2.0f);
		}
		bodies[0].body_ptr->SetLinearVelocity(teleportvelocity);
		teleportReady=false;

	}
	if (teleportSet) {
		teleportSet=false;
		teleportReady=true;
	}
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

Domino::Domino(b2World& world, float x, float y) : GameObject(world,x,y,"Domino", new DominoDrawable(x,y)) {
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x, y);
    b2Body* body_ptr = world.CreateBody(&bodyDef);
    b2PolygonShape dominoShape;
    dominoShape.SetAsBox(0.4, 1.5);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dominoShape;
    fixtureDef.density = 13.0f;
    fixtureDef.friction = 0.6f;
    body_ptr->CreateFixture(&fixtureDef);
	body_ptr->SetUserData(this);
	bodies.push_back(PhysBody(body_ptr, body_ptr->GetPosition()));
}

Platform::Platform(b2World& world, float x, float y, float _width, float _height) : GameObject(world,x,y,"Platform", new PlatformDrawable(x,y,_width,_height)), dimensions(_width, _height) { 

	b2BodyDef bodyDef;
	bodyDef.position.Set(x, y);
	b2Body* body_ptr = world.CreateBody(&bodyDef);
	b2Vec2 vertices[4];
	vertices[0].Set(PLATFORM_THRESHOLD, PLATFORM_THRESHOLD);
	vertices[1].Set(_width, _height + PLATFORM_THRESHOLD);
	vertices[2].Set(_width, _height + 2);
	vertices[3].Set(PLATFORM_THRESHOLD, 2);
	b2PolygonShape polygonShape;
	polygonShape.Set(vertices, 4);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &polygonShape;
	body_ptr->CreateFixture(&fixtureDef);
	bodies.push_back(PhysBody(body_ptr, body_ptr->GetPosition()));

	highlight_extras=true;
}
void Platform::highlightDelta(sf::Vector2i point) {
	b2PolygonShape* shape_ptr = dynamic_cast<b2PolygonShape*>(bodies[0].body_ptr->GetFixtureList()->GetShape());

	b2Vec2 vertices[4];
	for (int index=0;index<4;index++) {
		vertices[index]=shape_ptr->GetVertex(index);
	}

    b2Vec2 movementDelta(point.x / 10.0f - manipulationReferenceLocation.x, point.y / 10.0f - manipulationReferenceLocation.y);

    movementDelta.x -= std::fmod(movementDelta.x, DISCRETE_MOVE_UNIT);
    movementDelta.y -= std::fmod(movementDelta.y, DISCRETE_MOVE_UNIT);

    manipulationReferenceLocation.x += movementDelta.x;
    manipulationReferenceLocation.y += movementDelta.y;
    
	vertices[0] += movementDelta;
	vertices[1] += movementDelta;
    
	if (std::abs(vertices[1].y-vertices[2].y)/std::abs(vertices[1].x-vertices[2].x) < 1
		&& std::abs(vertices[1].x-vertices[2].x) > 3.0f && std::abs(vertices[1].x-vertices[2].x) < 40.0f) {
		shape_ptr->Set(vertices, 4);
        dimensions += movementDelta;
	}
	if (!noOverlaps()) {
		vertices[0] -= movementDelta;
		vertices[1] -= movementDelta;
        dimensions -= movementDelta;
		shape_ptr->Set(vertices,4);
	}
}

void Platform::move(float x, float y) {
    moveDiscretely(x, y);
}

b2Vec2 Platform::getDimensions() {
    return dimensions;
}

Wall::Wall(b2World& world, float x, float y, float _width, float _height) : GameObject(world,x,y,"Wall", new PlatformDrawable(x,y,_width,_height)), dimensions(_width, _height) {
	b2BodyDef bodyDef;
	bodyDef.position.Set(x, y);
	b2Body* body_ptr = world.CreateBody(&bodyDef);
	b2Vec2 vertices[4];
	vertices[0].Set(PLATFORM_THRESHOLD, PLATFORM_THRESHOLD);
	vertices[3].Set(2, PLATFORM_THRESHOLD);
	vertices[1].Set(_width + PLATFORM_THRESHOLD, _height);
	vertices[2].Set(_width +2, _height);
	b2PolygonShape polygonShape;
	polygonShape.Set(vertices, 4);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &polygonShape;
	body_ptr->CreateFixture(&fixtureDef);		
	bodies.push_back(PhysBody(body_ptr, body_ptr->GetPosition()));

	highlight_extras=true;
}
void Wall::highlightDelta(sf::Vector2i point) {
	b2PolygonShape* shape_ptr = dynamic_cast<b2PolygonShape*>(bodies[0].body_ptr->GetFixtureList()->GetShape());

	b2Vec2 vertices[4];
	for (int index=0;index<4;index++) {
		vertices[index]=shape_ptr->GetVertex(index);
	}
    
    //Because of the way B2D handles vertices, we have to check here which ones we want.
    size_t wanted_index;
    if (vertices[0].y < vertices[1].y) wanted_index=1;
    else wanted_index=0;
    
    b2Vec2 movementDelta(point.x / 10.0f - manipulationReferenceLocation.x, point.y / 10.0f - manipulationReferenceLocation.y);

    movementDelta.x -= std::fmod(movementDelta.x, DISCRETE_MOVE_UNIT);
    movementDelta.y -= std::fmod(movementDelta.y, DISCRETE_MOVE_UNIT);

    manipulationReferenceLocation.x += movementDelta.x;
    manipulationReferenceLocation.y += movementDelta.y;
    
	vertices[wanted_index] += movementDelta;
	vertices[wanted_index+1] += movementDelta;
    
	if (std::abs(vertices[wanted_index+1].x-vertices[wanted_index+2].x)/std::abs(vertices[wanted_index+1].y-vertices[wanted_index+2].y) < 1
		&& std::abs(vertices[wanted_index +1].y-vertices[wanted_index +2].y) > 3.0f && std::abs(vertices[wanted_index +1].y-vertices[wanted_index +2].y) < 40.0f) {
		shape_ptr->Set(vertices, 4);
        dimensions += movementDelta;
	}
    
	if (!noOverlaps()) {
		vertices[wanted_index] -= movementDelta;
		vertices[wanted_index+1] -= movementDelta;
		shape_ptr->Set(vertices,4);
        dimensions -= movementDelta;
	}
}
void Wall::move(float x, float y) {
    moveDiscretely(x, y);
}

b2Vec2 Wall::getDimensions() {
    return dimensions;
}

Catapult::Catapult(b2World& world, float x, float y, bool flipped) : GameObject(world, x,y,"Catapult", new CatapultDrawable(x,y, flipped)), flipped(flipped) {
	b2BodyDef bodyDef;
	bodyDef.position.Set(x, y);
	b2Body* body_ptr = world.CreateBody(&bodyDef);
	
	b2Vec2 vertices[3];
	vertices[0].Set(0, 0);
	vertices[1].Set(-1, 2);
	vertices[2].Set(1,2);
	b2PolygonShape polygonShape;
	polygonShape.Set(vertices, 3);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &polygonShape;
	body_ptr->CreateFixture(&fixtureDef);	
	bodies.push_back(PhysBody(body_ptr, body_ptr->GetPosition()));
	b2Body* body_ptr2;

	if (!flipped) {
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(x, y);
		bodyDef.angle = 0.4;
		body_ptr2 =world.CreateBody(&bodyDef);
		b2PolygonShape boxShape;
		boxShape.SetAsBox(5,0.2);		
		fixtureDef2.shape = &boxShape;
		fixtureDef2.density = 0.1;
		fixtureDef2.friction = 1;
		fixtureDef2.restitution = 0;
		body_ptr2->CreateFixture(&fixtureDef2);
		bodies.push_back(PhysBody(body_ptr2, body_ptr2->GetPosition(), body_ptr2->GetAngle()));

		boxShape.SetAsBox(0.2,1, b2Vec2(4.8,-1.2),0);
		fixtureDef2.shape = &boxShape;
		fixtureDef2.density = 1;
		fixtureDef2.friction = 1;
		fixtureDef2.restitution = 0;
		body_ptr2->CreateFixture(&fixtureDef2);
	}
	else {
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(x, y);
		bodyDef.angle = -0.4;
		body_ptr2 =world.CreateBody(&bodyDef);
		b2PolygonShape boxShape;
		boxShape.SetAsBox(5,0.2);
		fixtureDef2.shape = &boxShape;
		fixtureDef2.density = 0.1;
		fixtureDef2.friction = 1;
		fixtureDef2.restitution = 0;
		body_ptr2->CreateFixture(&fixtureDef2);
		bodies.push_back(PhysBody(body_ptr2, body_ptr2->GetPosition(), body_ptr2->GetAngle()));

		boxShape.SetAsBox(0.2,1, b2Vec2(-4.8,-1.2),0);
		fixtureDef2.shape = &boxShape;
		fixtureDef2.density = 1;
		fixtureDef2.friction = 1;
		fixtureDef2.restitution = 0;
		body_ptr2->CreateFixture(&fixtureDef2);
	}

	b2RevoluteJointDef jointDef;
	jointDef.localAnchorA.Set(0, 0);
	jointDef.localAnchorB.Set(0, 0);
	jointDef.bodyA = body_ptr;
	jointDef.bodyB = body_ptr2;
	jointDef.enableLimit = true;
	jointDef.lowerAngle = -0.4;
	jointDef.upperAngle =  0.4;
	world.CreateJoint(&jointDef);

	if (flipped) {
		bodies[1].body_ptr->SetTransform(bodies[1].body_ptr->GetPosition(), -0.4);
	}
}

std::string Catapult::highlightClicked(sf::Vector2i point) {
	std::string result=drawable->highlightClicked(point);
	if (result=="rotate") {
		if (bodies[1].body_ptr->GetAngle() > 0) {
			bodies[1].body_ptr->SetTransform(bodies[1].body_ptr->GetPosition(), -0.4);
			bodies[1].original_rot=-0.4;
			b2Fixture* fix = bodies[1].body_ptr->GetFixtureList();
			for (;fix!=NULL;fix=fix->GetNext()) {
				bodies[1].body_ptr->DestroyFixture(fix);
			}

			b2PolygonShape boxShape;
			boxShape.SetAsBox(5,0.2);
			fixtureDef2.shape = &boxShape;
			fixtureDef2.density = 0.1;
			fixtureDef2.friction = 1;
			fixtureDef2.restitution = 0;
			bodies[1].body_ptr->CreateFixture(&fixtureDef2);

			boxShape.SetAsBox(0.2,1, b2Vec2(-4.8,-1.2),0);
			fixtureDef2.shape = &boxShape;
			fixtureDef2.density = 1;
			fixtureDef2.friction = 1;
			fixtureDef2.restitution = 0;
			bodies[1].body_ptr->CreateFixture(&fixtureDef2);
			flipped = true;
			dynamic_cast<CatapultDrawable*>(drawable)->setFlipped(true);
		}
		else {
			bodies[1].body_ptr->SetTransform(bodies[1].body_ptr->GetPosition(), 0.4);
			bodies[1].original_rot=0.4;
			b2Fixture* fix = bodies[1].body_ptr->GetFixtureList();
			for (;fix!=NULL;fix=fix->GetNext()) {
				bodies[1].body_ptr->DestroyFixture(fix);
			}

			b2PolygonShape boxShape;
			boxShape.SetAsBox(5,0.2);
			fixtureDef2.shape = &boxShape;
			fixtureDef2.density = 0.1;
			fixtureDef2.friction = 1;
			fixtureDef2.restitution = 0;
			bodies[1].body_ptr->CreateFixture(&fixtureDef2);

			boxShape.SetAsBox(0.2,1, b2Vec2(4.8,-1.2),0);
			fixtureDef2.shape = &boxShape;
			fixtureDef2.density = 1;
			fixtureDef2.friction = 1;
			fixtureDef2.restitution = 0;
			bodies[1].body_ptr->CreateFixture(&fixtureDef2);
			flipped = false;
			dynamic_cast<CatapultDrawable*>(drawable)->setFlipped(false);
		}
		return "nothing";
	}
	return result;
	
}

Seesaw::Seesaw(b2World& world, float x, float y, bool flipped) : GameObject(world, x,y,"Seesaw", new SeesawDrawable(x,y)), flipped(flipped) {
	b2BodyDef bodyDef;
	bodyDef.position.Set(x, y);
	b2Body* body_ptr = world.CreateBody(&bodyDef);
	
	b2Vec2 vertices[3];
	vertices[0].Set(0, 0);
	vertices[1].Set(-1, 2);
	vertices[2].Set(1,2);
	b2PolygonShape polygonShape;
	polygonShape.Set(vertices, 3);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &polygonShape;
	body_ptr->CreateFixture(&fixtureDef);
	bodies.push_back(PhysBody(body_ptr, body_ptr->GetPosition()));
	
	
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(x, y);
	if (flipped) {
		bodyDef.angle = -0.4;
	}
	else {
		bodyDef.angle = 0.4;
	}
	b2Body* body_ptr2 =world.CreateBody(&bodyDef);
	b2PolygonShape boxShape;
	boxShape.SetAsBox(5,0.2);
	b2FixtureDef fixtureDef2;
	fixtureDef2.shape = &boxShape;
	fixtureDef2.density = 1;
	fixtureDef2.friction = 1;
	fixtureDef2.restitution = 0;
	body_ptr2->CreateFixture(&fixtureDef2);
	bodies.push_back(PhysBody(body_ptr2, body_ptr2->GetPosition(), body_ptr2->GetAngle()));

	b2RevoluteJointDef jointDef;
	jointDef.localAnchorA.Set(0, 0);
	jointDef.localAnchorB.Set(0, 0);
	jointDef.bodyA = body_ptr;
	jointDef.bodyB = body_ptr2;
	jointDef.enableLimit = true;
	jointDef.lowerAngle = -0.4;
	jointDef.upperAngle =  0.4;
	world.CreateJoint(&jointDef);
}


std::string Seesaw::highlightClicked(sf::Vector2i point) {
	std::string result=drawable->highlightClicked(point);
	if (result=="rotate") {
		if (bodies[1].body_ptr->GetAngle() > 0) {
			bodies[1].body_ptr->SetTransform(bodies[1].body_ptr->GetPosition(), -0.4);
			bodies[1].original_rot=-0.4;
			flipped = true;
		}
		else {
			bodies[1].body_ptr->SetTransform(bodies[1].body_ptr->GetPosition(), 0.4);
			bodies[1].original_rot=0.4;
			flipped = false;
		}
		return "nothing";
	}
	return result;
	
}



Crate::Crate(b2World& world, float x, float y) : GameObject(world, x, y,"Crate", new CrateDrawable(x,y)) {
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(x, y);
	b2Body* body_ptr = world.CreateBody(&bodyDef);
	b2PolygonShape boxShape;
	boxShape.SetAsBox(1.5,1.5);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1.0;
	fixtureDef.friction = 0.5;
	fixtureDef.restitution = 0.1;
	body_ptr->CreateFixture(&fixtureDef);
	body_ptr->SetUserData(this);
	bodies.push_back(PhysBody(body_ptr, body_ptr->GetPosition()));
}

Ball::Ball(b2World& world, float x, float y,std::string name, float r, float restitution = 0, float density = 1.0, Drawable* drawable= new Drawable) : GameObject(world, x,y,name, drawable) {
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(x, y);
	b2Body* body_ptr = world.CreateBody(&bodyDef);	
	b2CircleShape circleShape;
	circleShape.m_p.Set(0, 0);
	circleShape.m_radius = r;
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape;
	fixtureDef.density = density;
	fixtureDef.friction = 0.5;
	fixtureDef.restitution = restitution;
	body_ptr->CreateFixture(&fixtureDef);
	body_ptr->SetUserData(this);
	bodies.push_back(PhysBody(body_ptr, body_ptr->GetPosition(), body_ptr->GetAngle()));

}

BouncingBall::BouncingBall(b2World& world, float x, float y) : Ball(world, x, y,"BouncingBall", 0.5, 0.5, 1.0, new BouncingBallDrawable(x,y)) {}

BowlingBall::BowlingBall(b2World& world, float x, float y) : Ball(world, x, y,"BowlingBall", 1.0, 0.1, 3.0, new BowlingBallDrawable(x,y)) {}

BigBall::BigBall(b2World& world, float x, float y) : Ball(world, x, y,"BigBall", 2.0, 0.3, 0.4, new BigBallDrawable(x,y)) {}


Bomb::Bomb(b2World& world, float x, float y) : GameObject(world, x, y,"Bomb", new BombDrawable(x,y)) {
	b2BodyDef bodyDef;
	bodyDef.position.Set(x, y);
	b2Body* body_ptr = world.CreateBody(&bodyDef);
	b2PolygonShape boxShape;
	boxShape.SetAsBox(1.3,1.3);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	body_ptr->CreateFixture(&fixtureDef);
	exploded = false;
	body_ptr->SetUserData(this);
	bodies.push_back(PhysBody(body_ptr, body_ptr->GetPosition(), body_ptr->GetAngle()));
}

void Bomb::applyImpulse(b2Body* body, b2Vec2 center, b2Vec2 applyPoint, float power) {
      b2Vec2 blastDir = applyPoint - center;
      float distance = blastDir.Normalize();
      //Ignores the bomb
      if ( distance == 0 )
          return;
      float invDistance = 1 / distance;
      float impulseMag = power * invDistance * invDistance;
      body->ApplyLinearImpulse(impulseMag * blastDir, applyPoint, true);
}

void Bomb::explode() {
	if (exploded == false) {
		exploded = true;
		b2Vec2 center = bodies[0].body_ptr->GetPosition();
		float power = 300; 
		float blast_radius = 15;
		//apply impulse to bodies
		size_t rays = 50;
	    for (size_t i = 0; i < rays; i++) {
            float angle = (i / (float)rays) * 6.282;
            b2Vec2 rayDir( sinf(angle), cosf(angle) );
            b2Vec2 rayEnd = center + blast_radius * rayDir;

            //check what this ray hits
            RayCastClosestCallback callback;
            world.RayCast(&callback, center, rayEnd);
            if ( callback.m_body ) {
                applyImpulse(callback.m_body, center, callback.m_point, power);
            }
        }
	}	
}

bool Bomb::explodeStatus() const
{
	return exploded;
}

bool Bomb::isDestructable() const
{
	return true;
}

void Bomb::reset() {
	exploded = false;
	bodies[0].body_ptr->SetActive(true);
	dynamic_cast<BombDrawable*>(drawable)->setExploded(false);
	dynamic_cast<BombDrawable*>(drawable)->setExplosionStatus(0);
	GameObject::reset();
}

void Bomb::update_drawable() {
	drawable->update(bodies);
	if (exploded == true) {
		bodies[0].body_ptr->SetActive(false);
		dynamic_cast<BombDrawable*>(drawable)->setExploded(true);
	}	
}

GravityChanger::GravityChanger(b2World& world, float x, float y, bool flipped) : GameObject(world, x, y, "GravityChanger", new GravityChangerDrawable(x,y)), flipped(flipped) {
	b2BodyDef bodyDef;
	bodyDef.position.Set(x,y);
	b2Body* body_ptr=world.CreateBody(&bodyDef);
	b2PolygonShape shape1;
	shape1.SetAsBox(1.0f, 0.5f);
	check1=body_ptr->CreateFixture(&shape1,0.0f);
	b2PolygonShape shape2;
	shape2.SetAsBox(0.4f, 1.0f, b2Vec2(-1,-0.5f), 0);
	b2PolygonShape shape3;
	shape3.SetAsBox(0.4f, 1.0f, b2Vec2(1,-0.5f), 0);
	body_ptr->CreateFixture(&shape2,0);
	body_ptr->CreateFixture(&shape3,0);
	bodies.push_back(PhysBody(body_ptr, body_ptr->GetPosition(), body_ptr->GetAngle()));

	


	b2BodyDef bodyDef2;
	bodyDef2.position.Set(x,y-1.5f);
	bodyDef2.type = b2_dynamicBody;
	b2Body* button_ptr=world.CreateBody(&bodyDef2);
	button_ptr->SetGravityScale(0);
	b2PolygonShape buttonshape;
	buttonshape.SetAsBox(0.5f,0.5f);
	check2=button_ptr->CreateFixture(&buttonshape, 0);
	button_ptr->SetUserData(this);
	bodies.push_back(PhysBody(button_ptr, button_ptr->GetPosition(), button_ptr->GetAngle()));

	b2PrismaticJointDef prismDef;
	prismDef.bodyA = body_ptr;
	prismDef.localAnchorA = b2Vec2(0,-1);
	prismDef.bodyB = button_ptr;
	prismDef.localAnchorB = b2Vec2(0,0);
	prismDef.localAxisA = b2Vec2(0,1);
	prismDef.upperTranslation =1;
	prismDef.lowerTranslation =-0.5f;
	prismDef.enableLimit=true;
	prismDef.collideConnected=true;
	prismDef.enableMotor = true;
	prismDef.maxMotorForce=25;
	prismDef.motorSpeed=-3;
	world.CreateJoint(&prismDef);
	if (flipped) {
		bodies[0].body_ptr->SetTransform(bodies[0].original_pos-b2Vec2(0,1.5f), 3.14159);
		bodies[0].original_pos=bodies[0].body_ptr->GetTransform().p;
		bodies[0].original_rot=3.14159;

		bodies[1].body_ptr->SetTransform(bodies[1].original_pos+b2Vec2(0,1.5f), bodies[1].original_rot);
		bodies[1].original_pos=bodies[1].body_ptr->GetTransform().p;
	}
}

void GravityChanger::buttonCheck(b2Fixture* fixA, b2Fixture* fixB) {
	if ((fixA==check2 && fixB==check1) || (fixA==check1 && fixB==check2)) {
		world.SetGravity(-world.GetGravity());
		b2Body* body_list = world.GetBodyList();
		for (;body_list!=NULL;body_list=body_list->GetNext()) {
			body_list->SetAwake(true);
		}
	}
}
std::string GravityChanger::highlightClicked(sf::Vector2i point) {
	std::string result=drawable->highlightClicked(point);
	if (result=="rotate") {
		if (std::fabs(bodies[0].original_rot)<0.01f) {
			bodies[0].body_ptr->SetTransform(bodies[0].original_pos-b2Vec2(0,1.5f), 3.14159);
			bodies[0].original_pos=bodies[0].body_ptr->GetTransform().p;
			bodies[0].original_rot=3.14159;

			bodies[1].body_ptr->SetTransform(bodies[1].original_pos+b2Vec2(0,1.5f), bodies[1].original_rot);
			bodies[1].original_pos=bodies[1].body_ptr->GetTransform().p;
			flipped=true;
		}
		else {
			bodies[0].body_ptr->SetTransform(bodies[0].original_pos+b2Vec2(0,1.5f), 0);
			bodies[0].original_pos=bodies[0].body_ptr->GetTransform().p;
			bodies[0].original_rot=0;

			bodies[1].body_ptr->SetTransform(bodies[1].original_pos-b2Vec2(0,1.5f), bodies[1].original_rot);
			bodies[1].original_pos=bodies[1].body_ptr->GetTransform().p;
			flipped=false;
		}		
		return "nothing";
	}
	else return result;
}
Teleport::Teleport(b2World& world, float x, float y, int flipped) : GameObject(world, x, y, "Teleport", new TeleportDrawable(x,y)), flipped(flipped) {

	b2BodyDef bodyDef;
	bodyDef.position.Set(x, y);
	b2Body* body_ptr = world.CreateBody(&bodyDef);
	b2PolygonShape boxShape;
	boxShape.SetAsBox(4,0.2);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.isSensor=true;
	my_check=body_ptr->CreateFixture(&fixtureDef);
	body_ptr->SetUserData(this);
	bodies.push_back(PhysBody(body_ptr, body_ptr->GetPosition(), body_ptr->GetAngle()));

	b2PolygonShape leftShape;
	leftShape.SetAsBox(0.5,1, b2Vec2(-4.5,-0.8),0);
	b2FixtureDef leftDef;
	leftDef.shape= &leftShape;

	b2PolygonShape rightShape;
	rightShape.SetAsBox(0.5,1, b2Vec2(4.5,-0.8),0);
	b2FixtureDef rightDef;
	rightDef.shape= &rightShape;
	body_ptr->CreateFixture(&leftDef);
	body_ptr->CreateFixture(&rightDef);
	
	b2PolygonShape blocker;
	blocker.SetAsBox(5,1.3);
	b2FixtureDef blockerDef;
	blockerDef.shape = &blocker;
	blockerDef.isSensor=true;
	my_check2=body_ptr->CreateFixture(&blockerDef);
	


	teleports.push_back(this);
	//Rotate:
	if (flipped==0) {
		bodies[0].body_ptr->SetTransform(bodies[0].body_ptr->GetPosition(),0);
	}
	if (flipped==3) {
		bodies[0].body_ptr->SetTransform(bodies[0].body_ptr->GetPosition(),3.14159f/2.0f);
	}
	if (flipped==2) {
		bodies[0].body_ptr->SetTransform(bodies[0].body_ptr->GetPosition(),3.14159f);
	}
	if (flipped==1) {
		bodies[0].body_ptr->SetTransform(bodies[0].body_ptr->GetPosition(),3.0f*3.14159f/2.0f);
	}
	bodies[0].original_rot=bodies[0].body_ptr->GetAngle();
}
Teleport::~Teleport() {
	teleports.remove(this);
}
GameObject* Teleport::calculate_next() {
	if (teleports.size()==1) return NULL;
	std::list<GameObject*>::iterator it = find(teleports.begin(), teleports.end(), this);
	if (*it==teleports.back()) {
		return teleports.front();
	}	
	else {
		it++;
		return *it;
	}
}
void Teleport::beginContact(GameObject* go_ptr, b2Fixture* check) {
	if (go_ptr==NULL) {
		return;
	}
	if (check!=my_check) return;
	if (find(incoming.begin(), incoming.end(), go_ptr) == incoming.end()) {
		Teleport* next=dynamic_cast<Teleport*>(calculate_next());
		if (next!=NULL) {
			next->notify(go_ptr);
			b2Vec2 target=next->getCurrentPos();
			target+=go_ptr->getCurrentPos()-getCurrentPos();

			int flipped_delta=next->flipped-flipped;
			if (flipped_delta<0) flipped_delta=4+flipped_delta;
			b2Vec2 targetvelocity=go_ptr->getLinearVelocity();
			for (;flipped_delta>0;flipped_delta--) {
				targetvelocity=b2Vec2(targetvelocity.y,-targetvelocity.x);
			}

			flipped_delta=next->flipped-flipped;
			if (flipped_delta%2==0) {
				go_ptr->teleport(target, targetvelocity, false);
			}
			else {
				go_ptr->teleport(target, targetvelocity, true);
			}
		}
	}
}
void Teleport::endContact(GameObject* go_ptr, b2Fixture* check) {
	if (check!=my_check2) return;
	if (find(incoming.begin(), incoming.end(), go_ptr) != incoming.end()) {
		incoming.remove(go_ptr);
	}
}
void Teleport::notify(GameObject* go_ptr) {
	incoming.push_back(go_ptr);
}

std::list<GameObject*> Teleport::teleports;

std::string Teleport::highlightClicked(sf::Vector2i point) {
	std::string result=drawable->highlightClicked(point);
	if (result=="rotate") {
		flipped++;
		if (flipped==4) flipped=0;
		if (flipped==0) {
			bodies[0].body_ptr->SetTransform(bodies[0].body_ptr->GetPosition(),0);
		}
		if (flipped==3) {
			bodies[0].body_ptr->SetTransform(bodies[0].body_ptr->GetPosition(),3.14159f/2.0f);
		}
		if (flipped==2) {
			bodies[0].body_ptr->SetTransform(bodies[0].body_ptr->GetPosition(),3.14159f);
		}
		if (flipped==1) {
			bodies[0].body_ptr->SetTransform(bodies[0].body_ptr->GetPosition(),3.0f*3.14159f/2.0f);
		}
		if (!noOverlaps()) {
			reset();
		}
		
		else {
			bodies[0].original_rot=bodies[0].body_ptr->GetAngle();
		}
			
	return "nothing";
	}
	else return result;
}

void Teleport::reset() {
	incoming.clear();
}
