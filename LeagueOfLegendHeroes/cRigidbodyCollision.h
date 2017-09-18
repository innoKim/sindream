#pragma once
class cRigidbody;

#define EPSILON_COLLISION 1.0f

enum eCollisionState
{
	COLLISION_NONE,
	COLLISION_CONTACT,
	COLLISION_OVERLAPPED
};

class cRigidbodyCollision
{
	SYNTHESIZE(cRigidbody*, m_pObj1, Object1);
	SYNTHESIZE(cRigidbody*, m_pObj2, Object2);

	SYNTHESIZE(eCollisionState*, m_eCollisionState, CollisionState);

	cRigidbodyCollision();

public:
	cRigidbodyCollision(cRigidbody* object1, cRigidbody* object2);
	virtual ~cRigidbodyCollision();

	eCollisionState FindCollisionState();
	bool CalculateReaction();
};

