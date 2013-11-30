#include "GameObject.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <SFML/System.hpp>
#include <cmath>
#include <iostream>


GameObject::~GameObject() {
	body_ptr->GetWorld()->DestroyBody(body_ptr);
	body_ptr=NULL;
	delete drawable;
	drawable=NULL;
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
	if (name=="BouncingBall")
		return new BouncingBall(world, x, y);
	if (name=="Seesaw")
		return new Seesaw(world, x, y);
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

Domino::Domino(b2World& world, float x, float y) : GameObject(world,x,y,"Domino") {
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

Platform::Platform(b2World& world, float x, float y, float width, float height) : GameObject(world,x,y,"Platform", new PlatformDrawable(x,y,width,height)) { 

	b2BodyDef bodyDef;
	bodyDef.position.Set(x, y);
	body_ptr = world.CreateBody(&bodyDef);
	b2Vec2 vertices[4];
	vertices[0].Set(0, 0);
	vertices[1].Set(width, height);
	vertices[2].Set(width, height + 2);
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


Wall::Wall(b2World& world, float x, float y, float width, float height) : GameObject(world,x,y,"Wall", new PlatformDrawable(x,y,width,height)) {
	b2BodyDef bodyDef;
	bodyDef.position.Set(x, y);
	body_ptr = world.CreateBody(&bodyDef);
	b2Vec2 vertices[4];
	vertices[0].Set(0, 0);
	vertices[3].Set(2, 0);
	vertices[1].Set(width, height);
	vertices[2].Set(width +2, height);
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

Catapult::Catapult(b2World& world, float x, float y) : GameObject(world, x,y,"Catapult") {
	b2BodyDef bodyDef;
	bodyDef.position.Set(x, y);
	body_ptr = world.CreateBody(&bodyDef);
	
	b2Vec2 vertices[3];
	vertices[0].Set(0, 0);
	vertices[1].Set(-1, 2);
	vertices[2].Set(1,2);
	b2PolygonShape polygonShape;
	polygonShape.Set(vertices, 3);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &polygonShape;
	body_ptr->CreateFixture(&fixtureDef);
	
	
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(x, y);
	bodyDef.angle = 0;
	original_rot = 0;
	body_ptr2 =world.CreateBody(&bodyDef);
	b2PolygonShape boxShape;
	boxShape.SetAsBox(5,0.2);
	b2FixtureDef fixtureDef2;
	fixtureDef2.shape = &boxShape;
	fixtureDef2.density = 1;
	fixtureDef2.friction = 1;
	fixtureDef2.restitution = 0;
	body_ptr2->CreateFixture(&fixtureDef2);

	boxShape.SetAsBox(0.2,1, b2Vec2(4.8,-1.2),0);
	fixtureDef2.shape = &boxShape;
	fixtureDef2.density = 1;
	fixtureDef2.friction = 1;
	fixtureDef2.restitution = 0;
	body_ptr2->CreateFixture(&fixtureDef2);

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

Seesaw::Seesaw(b2World& world, float x, float y) : GameObject(world, x,y,"Seesaw") {
	b2BodyDef bodyDef;
	bodyDef.position.Set(x, y);
	body_ptr = world.CreateBody(&bodyDef);
	
	b2Vec2 vertices[3];
	vertices[0].Set(0, 0);
	vertices[1].Set(-1, 2);
	vertices[2].Set(1,2);
	b2PolygonShape polygonShape;
	polygonShape.Set(vertices, 3);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &polygonShape;
	body_ptr->CreateFixture(&fixtureDef);
	
	
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(x, y);
	bodyDef.angle = 0.4;
	original_rot = 0.4;
	body_ptr2 =world.CreateBody(&bodyDef);
	b2PolygonShape boxShape;
	boxShape.SetAsBox(5,0.2);
	b2FixtureDef fixtureDef2;
	fixtureDef2.shape = &boxShape;
	fixtureDef2.density = 1;
	fixtureDef2.friction = 1;
	fixtureDef2.restitution = 0;
	body_ptr2->CreateFixture(&fixtureDef2);

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

void Seesaw::reset() {
	body_ptr->SetTransform(original_pos, 0);
	body_ptr->SetLinearVelocity(b2Vec2(0,0));
	body_ptr->SetAngularVelocity(0);
	body_ptr->SetAwake(true);
	body_ptr2->SetTransform(original_pos, original_rot);
	body_ptr2->SetLinearVelocity(b2Vec2(0,0));
	body_ptr2->SetAngularVelocity(0);
	body_ptr2->SetAwake(true);
}

Ball::Ball(b2World& world, float x, float y,std::string name, float r, float restitution = 0, float density = 1.0) : GameObject(world, x,y,name) {
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
	fixtureDef.friction = 0.5;
	fixtureDef.restitution = restitution;
	body_ptr->CreateFixture(&fixtureDef);
}

BouncingBall::BouncingBall(b2World& world, float x, float y) : Ball(world, x, y,"BouncingBall", 0.5, 0.5, 1.0) {}

BowlingBall::BowlingBall(b2World& world, float x, float y) : Ball(world, x, y,"BowlingBall", 0.8, 0.1, 3.0) {}

BigBall::BigBall(b2World& world, float x, float y) : Ball(world, x, y,"BigBall", 2.0, 0.1, 0.4) {}


Bomb::Bomb(b2World& world, float x, float y) : Ball(world, x, y,"Bomb", 0.8, 0, 5.0) {
	exploded = false;
	body_ptr->SetUserData(this);
}

void Bomb::applyImpulse(b2Body* body, b2Vec2 center, b2Vec2 applyPoint, float power) {
	std::cout << "applyimpulse\n";
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
		std::cout << "bomb explode\n";
		exploded = true;
		b2Vec2 center = body_ptr->GetPosition();
		//find all fixtures within blast radius AABB
		MyQueryCallback queryCallback;
		b2AABB aabb;
		float blast_radius = 300;
		aabb.lowerBound = center - b2Vec2(blast_radius, blast_radius);
		aabb.upperBound = center + b2Vec2(blast_radius, blast_radius);
	   	world.QueryAABB(&queryCallback, aabb);
		float power = 300;
		//apply impulse to bodies
		for (size_t i = 0; i < queryCallback.foundBodies.size(); i++) {
		    b2Body* body = queryCallback.foundBodies[i];
		    b2Vec2 body_pos = body->GetWorldCenter();
		    //ignore bodies outside the blast range
		    if ((body_pos - center).Length() >= blast_radius)
		        continue;
		    applyImpulse(body, center, body_pos, power * 0.5f );
		}
	}
} 

void Bomb::reset() {
	body_ptr->SetTransform(original_pos, original_rot);
	body_ptr->SetLinearVelocity(b2Vec2(0,0));
	body_ptr->SetAngularVelocity(0);
	body_ptr->SetAwake(true);
	exploded = false;
}


Lift::Lift(b2World& world, float x1, float y1, float x2, float y2) : GameObject(world, x1, y1, "Lift") {
	b2BodyDef bodyDef;
	bodyDef.position.Set(x1, y1);
	bodyDef.type = b2_dynamicBody;
	body_ptr = world.CreateBody(&bodyDef);
	b2PolygonShape boxShape;
	boxShape.SetAsBox(2,0.3);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1;
	fixtureDef.friction = 1;
	fixtureDef.restitution = 0;
	body_ptr->CreateFixture(&fixtureDef);

	bodyDef.position.Set(x2, y2);
	body_ptr2 = world.CreateBody(&bodyDef);
	body_ptr2->CreateFixture(&fixtureDef);

	b2Vec2 anchor1 = body_ptr->GetWorldCenter();
	b2Vec2 anchor2 = body_ptr2->GetWorldCenter();
	b2Vec2 groundAnchor1(x1, y1 - 15.0f);
	b2Vec2 groundAnchor2(x2, y2 - 15.0f);
	float32 ratio = 1.0f;
	b2PulleyJointDef jointDef;
	jointDef.Initialize(body_ptr, body_ptr2, groundAnchor1, groundAnchor2, anchor1, anchor2, ratio);
}



GravityChanger::GravityChanger(b2World& world, float x, float y) : GameObject(world, x, y, "GravityChanger") {
	b2BodyDef bodyDef;
	bodyDef.position.Set(x,y);
	body_ptr=world.CreateBody(&bodyDef);
	b2PolygonShape shape1;
	shape1.SetAsBox(1.0f, 0.5f);
	check1=body_ptr->CreateFixture(&shape1,0.0f);
	b2PolygonShape shape2;
	shape2.SetAsBox(0.4f, 1.0f, b2Vec2(-1,-0.5f), 0);
	b2PolygonShape shape3;
	shape3.SetAsBox(0.4f, 1.0f, b2Vec2(1,-0.5f), 0);
	body_ptr->CreateFixture(&shape2,0);
	body_ptr->CreateFixture(&shape3,0);

	


	b2BodyDef bodyDef2;
	bodyDef2.position.Set(x,y-1.5f);
	bodyDef2.type = b2_dynamicBody;
	button_ptr=world.CreateBody(&bodyDef2);
	button_ptr->SetGravityScale(0);
	b2PolygonShape buttonshape;
	buttonshape.SetAsBox(0.5f,0.5f);
	check2=button_ptr->CreateFixture(&buttonshape, 0);
	button_ptr->SetUserData(this);

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

}

void GravityChanger::buttonCheck(b2Fixture* fixA, b2Fixture* fixB) {
	if ((fixA==check2 && fixB==check1) || (fixA==check1 && fixB==check2)) {
		world.SetGravity(-world.GetGravity());
		std::cout << "Button!\n";
	}
}

/*
Teleport::Teleport(b2World& world, float x1, float y1, float x2, float y2) : GameObject(world, x1, y1) {
	//contacting = false;
	body_ptr->SetUserData(this);

	b2BodyDef bodyDef;
	bodyDef.position.Set(x1, y1);
	body_ptr = world.CreateBody(&bodyDef);
	b2PolygonShape boxShape;
	boxShape.SetAsBox(2,0.2);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	body_ptr->CreateFixture(&fixtureDef);

	bodyDef.position.Set(x2, y2);
	body_ptr2 = world.CreateBody(&bodyDef);
	body_ptr2->CreateFixture(&fixtureDef);
}

*/

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
