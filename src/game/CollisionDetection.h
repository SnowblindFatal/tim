
#ifndef COLLISION_H
#define COLLISION_H

class CollisionDetection : public b2ContactListener {
	void BeginContact(b2Contact* contact) {
		//check if fixture A was a bomb
		void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
		if ( bodyUserData )
			static_cast<Bomb*>( bodyUserData )->startContact();

		//check if fixture B was a bomb
		bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
		if ( bodyUserData )
			static_cast<Bomb*>( bodyUserData )->startContact();
	}

	void EndContact(b2Contact* contact) {
		//check if fixture A was a bomb
		void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
		if ( bodyUserData )
			static_cast<Bomb*>( bodyUserData )->endContact();
		//check if fixture B was a bomb
		bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
		if ( bodyUserData )
			static_cast<Bomb*>( bodyUserData )->endContact();
	}
};





#endif
