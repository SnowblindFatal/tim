#ifndef PHYS_BODY_H
#define PHYS_BODY_H
#include <Box2D/Box2D.h>
class PhysBody {
public:
	PhysBody(b2Body* body_ptr=NULL, b2Vec2 original_pos=b2Vec2(0,0), float original_rot=0) : body_ptr(body_ptr), original_pos(original_pos), original_rot(original_rot) {}
	b2Body* body_ptr;
	b2Vec2 original_pos;
	float original_rot;
	//b2Vec2 local_mouse;
};

#endif
