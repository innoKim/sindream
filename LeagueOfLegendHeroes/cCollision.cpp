#include "stdafx.h"
#include "cCollision.h"
#include "cPhysics.h"

cCollision::cCollision()
{
}

cCollision::~cCollision()
{
}

void cCollision::Destroy()
{
}

void cCollision::ObjVSObj(cPhysics & obj1, cPhysics & obj2, bool byObstacle)
{
	D3DXVECTOR3 n = FindNormalToCollisionPlane(obj1, obj2);

	if (obj1.GetIsActivate() && obj1.GetIsActivate())
	{
		D3DXVECTOR3 n = FindNormalToCollisionPlane(obj1, obj2);

		float v1 = D3DXVec3Dot(&obj1.GetVelocity(), &n);
		float v2 = D3DXVec3Dot(&obj2.GetVelocity(), &n);
		float e = (obj1.GetElastic() + obj2.GetElastic()) / 2;

		float fv1, fv2;
		FindCollidedVelocity(fv1, fv2, obj1, obj2);

		D3DXVECTOR3 vV1 = (fv1 - v1)*n + obj1.GetVelocity();
		D3DXVECTOR3 vV2 = (fv2 - v2)*n + obj2.GetVelocity();

		obj1.SetVelocity(vV1);
		obj2.SetVelocity(vV2);

		if (!byObstacle)
		{
			obj1.SetRestDuration(0.0f);
			obj2.SetRestDuration(0.0f);
		}
	}

	//가까우면 부담스러우니까 떨어트려
	if (obj1.GetIsActivate())(*obj1.GetPositionPtr() += n*obj1.GetRadius()*1.0f);
	if (obj2.GetIsActivate())(*obj2.GetPositionPtr() -= n*obj2.GetRadius()*1.0f);
}

void cCollision::ObjVSObstacle(cPhysics& obj)
{
	//벽이랑 충돌할때에는 충돌점에 잠시 물리요소를 생성해서 일반 충돌과 똑같이 작동하도록
	cPhysics obstacle;
	D3DXVECTOR3 obsPos = *obj.GetPositionPtr();
	obstacle.SetPositionPtr(&D3DXVECTOR3(obsPos.x, obj.Height(),obsPos.z));
	obstacle.SetMass(OBSTACLE_MASS);

	ObjVSObj(obj, obstacle,true);

	(*obj.GetPositionPtr()).y = obj.Height();
}

void cCollision::ObjVSWall(cPhysics& obj)
{
	//벽이랑 충돌할때에는 충돌점에 잠시 물리요소를 생성해서 일반 충돌과 똑같이 작동하도록
	cPhysics obstacle;
	D3DXVECTOR3 obsPos = *obj.GetPositionPtr();
	D3DXVECTOR3 dir;
	D3DXVec3Normalize(&dir,&obstacle.GetVelocity());
	obstacle.SetPositionPtr(&(obsPos+dir));
	obstacle.SetRadius(obj.GetRadius());
	obstacle.SetMass(OBSTACLE_MASS);

	ObjVSObj(obj, obstacle, true);
}


void cCollision::FindCollidedVelocity(OUT float & v1, OUT float & v2, IN cPhysics & obj1, IN cPhysics & obj2)
{
	float m1 = obj1.GetMass(), m2 = obj2.GetMass();
	float u1 = D3DXVec3Length(&obj1.GetVelocity()), u2 = D3DXVec3Length(&obj2.GetVelocity());
	float e = (obj1.GetElastic() + obj2.GetElastic()) / 2;

	v1 = ((m1 - e*m2)*u1 + (1 + e)*m2*u2) / (m1 + m2);
	v2 = ((m2 - e*m1)*u2 + (1 + e)*m1*u1) / (m1 + m2);
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


float cCollision::FindMomentumCharge(float v, cPhysics & obj)
{
	return fabs(obj.GetMass()*v + obj.GetMass()*D3DXVec3Length(&obj.GetVelocity()));
}