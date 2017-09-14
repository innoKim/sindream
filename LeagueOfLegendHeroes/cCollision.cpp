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

void cCollision::ObjVSObj(cPhysics & obj1, cPhysics & obj2)
{
	float dirFactor = D3DXVec3Dot(&obj1.GetVelocity(), &obj2.GetVelocity());

	float fv1, fv2;
	float c1, c2;
	D3DXVECTOR3 n, v1, v2, dirPlus, dirMinus;

	FindCollidedVelocity(fv1, fv2, obj1, obj2);
	
	c1 = FindMomentumCharge(fv1, obj1);
	c2 = FindMomentumCharge(fv2, obj2);

	if (dirFactor > 0) //같은방향
	{
		D3DXVECTOR3 vObj2toObj1 = (*obj1.GetPositionPtr()) - (*obj2.GetPositionPtr());
		
		if (D3DXVec3Dot(&vObj2toObj1, &obj1.GetVelocity())>0)// 1이 앞쪽일때
		{
			n = FindNormalToCollisionPlane(obj1, obj2);

			FindVectorCollided(v1, v2, n, obj1, obj2);

			dirPlus = v1 + n*c1 / obj1.GetMass();
			dirMinus = v2 + n*c2 / obj2.GetMass();

			D3DXVec3Normalize(&dirPlus, &dirPlus);
			D3DXVec3Normalize(&dirMinus, &dirMinus);

			obj1.SetVelocity(fv1*dirPlus);
			obj2.SetVelocity(fv2*dirMinus);

			return;
		}
		else //2가 앞쪽일때
		{
			n = FindNormalToCollisionPlane(obj2, obj1);
			FindVectorCollided(v2, v1, n, obj2, obj1);

			dirPlus = v2 + n*c2 / obj2.GetMass();
			dirMinus = v1 + n*c1 / obj1.GetMass();

			D3DXVec3Normalize(&dirPlus, &dirPlus);
			D3DXVec3Normalize(&dirMinus, &dirMinus);

			obj1.SetVelocity(fv1*dirMinus);
			obj2.SetVelocity(fv2*dirPlus);

			return;
		}
	}
	else //다른 방향
	{
		if (obj1.GetVelocity()*obj1.GetMass() < obj2.GetVelocity()*obj2.GetMass())
		{
			n = FindNormalToCollisionPlane(obj1, obj2);

			FindVectorCollided(v1, v2, n, obj1, obj2);

			dirPlus = v1 + n*c1 / obj1.GetMass();
			dirMinus = v2 + n*c2 / obj2.GetMass();

			D3DXVec3Normalize(&dirPlus, &dirPlus);
			D3DXVec3Normalize(&dirMinus, &dirMinus);

			obj1.SetVelocity(fv1*dirPlus);
			obj2.SetVelocity(fv2*dirMinus);

			return;
		}
		else
		{
			n = FindNormalToCollisionPlane(obj2, obj1);
			FindVectorCollided(v2, v1, n, obj2, obj1);

			dirPlus = v2 + n*c2 / obj2.GetMass();
			dirMinus = v1 + n*c1 / obj1.GetMass();

			D3DXVec3Normalize(&dirPlus, &dirPlus);
			D3DXVec3Normalize(&dirMinus, &dirMinus);

			obj1.SetVelocity(fv1*dirMinus);
			obj2.SetVelocity(fv2*dirPlus);
			return;
		}
	}
}

void cCollision::ObjVSObstacle(cPhysics& obj)
{
	//바닥과의 충돌 하늘에서 떨어졌을때
	D3DXVECTOR3 n(0, 1, 0);
	
	if (D3DXVec3Dot(&n, &obj.GetVelocity()) < 0) // 위에서 아래로 떨어지면 튕긴다.
	{
		D3DXVECTOR3 temp = n*D3DXVec3Dot(&n, &obj.GetVelocity());

		obj.SetVelocity(obj.GetElastic()*(obj.GetVelocity() + 2 * temp));
	}
	
	//아님 말구
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
