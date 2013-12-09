
#ifndef COLLISION_H
#define COLLISION_H
#include "GameObject.h"
#include <iostream>

class CollisionDetection : public b2ContactListener {
	void BeginContact(b2Contact* contact) {
		if (contact->GetFixtureA()->GetBody()->GetUserData() !=NULL) {
			GameObject* objectA= static_cast<GameObject*>(contact->GetFixtureA()->GetBody()->GetUserData());
			if ( objectA->getName()=="Bomb") {
				objectA->explode();
			}
			if ( objectA->getName()=="GravityChanger" ) {
				GravityChanger* GravityObjectA=dynamic_cast<GravityChanger*>(objectA);
				GravityObjectA->buttonCheck(contact->GetFixtureA(),contact->GetFixtureB());
			}
			if ( objectA->getName()=="Teleport" ) {
				Teleport* TeleportA=dynamic_cast<Teleport*>(objectA);
				TeleportA->beginContact(static_cast<GameObject*>(contact->GetFixtureB()->GetBody()->GetUserData()), contact->GetFixtureA());
			}
			
		}
		if (contact->GetFixtureB()->GetBody()->GetUserData() !=NULL) {
			GameObject* objectB = static_cast<GameObject*>(contact->GetFixtureB()->GetBody()->GetUserData());
			if ( objectB->getName()=="Bomb") {
				objectB->explode();
			}
			
			if ( objectB->getName()=="GravityChanger" ) {
				GravityChanger* GravityObjectB=dynamic_cast<GravityChanger*>(objectB);
				GravityObjectB->buttonCheck(contact->GetFixtureA(),contact->GetFixtureB());
			}
			if ( objectB->getName()=="Teleport" ) {
				Teleport* TeleportB=dynamic_cast<Teleport*>(objectB);
				TeleportB->beginContact(static_cast<GameObject*>(contact->GetFixtureA()->GetBody()->GetUserData()), contact->GetFixtureB());
			}
			
		}
	}
	void EndContact(b2Contact* contact) {
		if (contact->GetFixtureA()->GetBody()->GetUserData() !=NULL) {
			GameObject* objectA= static_cast<GameObject*>(contact->GetFixtureA()->GetBody()->GetUserData());
			if ( objectA->getName()=="Teleport" ) {
				Teleport* TeleportA=dynamic_cast<Teleport*>(objectA);
				TeleportA->endContact(static_cast<GameObject*>(contact->GetFixtureB()->GetBody()->GetUserData()), contact->GetFixtureA());
			}
			
		}
		if (contact->GetFixtureB()->GetBody()->GetUserData() !=NULL) {
			GameObject* objectB = static_cast<GameObject*>(contact->GetFixtureB()->GetBody()->GetUserData());
			if ( objectB->getName()=="Teleport" ) {
				Teleport* TeleportB=dynamic_cast<Teleport*>(objectB);
				TeleportB->endContact(static_cast<GameObject*>(contact->GetFixtureA()->GetBody()->GetUserData()), contact->GetFixtureB());
			}
			
		}
		
	}
};





#endif
