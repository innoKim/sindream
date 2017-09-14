#include "stdafx.h"
#include "cCollision.h"
#include "cPhysics.h"

cCollision::cCollision()
{
}

cCollision::~cCollision()
{
}

void cCollision::ObjVSObj(cPhysics & obj1, cPhysics & obj2)
{
}

void cCollision::ObjVSObstacle()
{
}

void cCollision::FindCollidedVelocity(OUT float & v1, OUT float & v2, IN cPhysics & obj1, IN cPhysics & obj2)
{
	float m1 = obj1.GetMass(), m2 = obj2.GetMass();
	float u1 = D3DXVec3Length(&obj1.GetVelocity()), u2 = D3DXVec3Length(&obj2.GetVelocity());
	float e = (obj1.GetElastic() + obj2.GetElastic()) / 2;

	v1 = ((m1 - e*m2)*u1 + (1 + e)*m2*u2) / (m1 + m2);
	v2 = ((m2 - e*m1)*u2 + (1 + e)*m1*u1) / (m1 + m2);
}

float cCollision::FindMomentumCharge(float v, cPhysics & obj)
{
	return fabs(obj.GetMass()*v + obj.GetMass()*D3DXVec3Length(&obj.GetVelocity()));
}

D3DXVECTOR3 cCollision::FindNormalToCollisionPlane(cPhysics & obj1, cPhysics & obj2)
{
	D3DXVECTOR3 norm;
	D3DXVec3Normalize(&norm, &((*obj1.GetPositionPtr()) - (*obj2.GetPositionPtr())));

	return norm;
}

void cCollision::FindVectorCollided(OUT D3DXVECTOR3 & vPlus, OUT D3DXVECTOR3 & vMinus, IN D3DXVECTOR3 norm, IN cPhysics & objPlus, IN cPhysics & objMinus)
{
	vPlus = objPlus.GetVelocity() + (2 * norm * D3DXVec3Dot(&objPlus.GetVelocity(), &norm));
	vMinus = objMinus.GetVelocity() + (2 * (-norm) * D3DXVec3Dot(&objMinus.GetVelocity(), &(-norm)));
}
