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
//
//void cCollision::ObjVSObj(cPhysics & obj1, cPhysics & obj2)
//{
//	//하나만 속도가 있을때
//	if (obj2.GetVelocityScalar() < EPSILON_VELOCITY) // 1이 움직이고있다.
//	{	
//		cPhysics* pobj1 = &obj1;
//		cPhysics* pobj2 = &obj2;
//		
//		float e = (pobj1->GetElastic() + pobj2->GetElastic()) / 2.0f;
//
//		//v1' = e*v1
//		float fv1 = e*pobj1->GetVelocityScalar();
//		//v2' = (1-e)*m1*v1/m2
//		float fv2 = (1 - e)*pobj1->GetMass()*pobj1->GetVelocityScalar() / pobj2->GetMass();
//
//		D3DXVECTOR3 n = FindNormalToCollisionPlane(obj2, obj1);
//
//		objPlus.GetVelocity() + (2 * norm * D3DXVec3Dot(&objPlus.GetVelocity(), &norm))
//	}
//	else if (obj1.GetVelocityScalar() < EPSILON_VELOCITY)// 2가 움직이고있다.
//	{
//
//	}
//	//둘다 속도가 있을때
//	else 
//	{
//		float dirFactor = D3DXVec3Dot(&obj1.GetVelocity(), &obj2.GetVelocity());
//
//		float fv1, fv2;
//		float c1, c2;
//		D3DXVECTOR3 n, v1, v2, dirPlus, dirMinus;
//
//		FindCollidedVelocity(fv1, fv2, obj1, obj2);
//
//		c1 = FindMomentumCharge(fv1, obj1);
//		c2 = FindMomentumCharge(fv2, obj2);
//
//		if (dirFactor > 0) //같은방향
//		{
//			D3DXVECTOR3 vObj2toObj1 = (*obj1.GetPositionPtr()) - (*obj2.GetPositionPtr());
//
//			if (D3DXVec3Dot(&vObj2toObj1, &obj1.GetVelocity())>0)// 1이 앞쪽일때
//			{
//				n = FindNormalToCollisionPlane(obj1, obj2);
//
//				FindVectorCollided(v1, v2, n, obj1, obj2);
//
//				dirPlus = v1 + n*c1 / obj1.GetMass();
//				dirMinus = v2 + n*c2 / obj2.GetMass();
//
//				D3DXVec3Normalize(&dirPlus, &dirPlus);
//				D3DXVec3Normalize(&dirMinus, &dirMinus);
//
//				obj1.SetVelocity(fv1*dirPlus);
//				obj2.SetVelocity(fv2*dirMinus);
//			}
//			else //2가 앞쪽일때
//			{
//				n = FindNormalToCollisionPlane(obj2, obj1);
//				FindVectorCollided(v2, v1, n, obj2, obj1);
//
//				dirPlus = v2 + n*c2 / obj2.GetMass();
//				dirMinus = v1 + n*c1 / obj1.GetMass();
//
//				D3DXVec3Normalize(&dirPlus, &dirPlus);
//				D3DXVec3Normalize(&dirMinus, &dirMinus);
//
//				obj1.SetVelocity(fv1*dirMinus);
//				obj2.SetVelocity(fv2*dirPlus);
//			}
//		}
//		else //다른 방향
//		{
//			if (obj1.GetVelocity()*obj1.GetMass() < obj2.GetVelocity()*obj2.GetMass())
//			{
//				n = FindNormalToCollisionPlane(obj1, obj2);
//
//				FindVectorCollided(v1, v2, n, obj1, obj2);
//
//				dirPlus = v1 + n*c1 / obj1.GetMass();
//				dirMinus = v2 + n*c2 / obj2.GetMass();
//
//				D3DXVec3Normalize(&dirPlus, &dirPlus);
//				D3DXVec3Normalize(&dirMinus, &dirMinus);
//
//				obj1.SetVelocity(fv1*dirPlus);
//				obj2.SetVelocity(fv2*dirMinus);
//			}
//			else
//			{
//				n = FindNormalToCollisionPlane(obj2, obj1);
//				FindVectorCollided(v2, v1, n, obj2, obj1);
//
//				dirPlus = v2 + n*c2 / obj2.GetMass();
//				dirMinus = v1 + n*c1 / obj1.GetMass();
//
//				D3DXVec3Normalize(&dirPlus, &dirPlus);
//				D3DXVec3Normalize(&dirMinus, &dirMinus);
//
//				obj1.SetVelocity(fv1*dirMinus);
//				obj2.SetVelocity(fv2*dirPlus);
//			}
//		}
//
//		obj1.SetRestDuration(0.0f);
//		obj2.SetRestDuration(0.0f);
//
//		obj1.SetIsActivate(false);
//		obj2.SetIsActivate(false);
//	}	
//}

void cCollision::ObjVSObj(cPhysics & obj1, cPhysics & obj2)
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

	obj1.SetRestDuration(0.0f);
	obj2.SetRestDuration(0.0f);

	//가까우면 부담스러우니까 떨어트려
	(*obj1.GetPositionPtr() += n*obj1.GetRadius()*0.1f);
	(*obj2.GetPositionPtr() -= n*obj2.GetRadius()*0.1f);
}

void cCollision::ObjVSObstacle(cPhysics& obj)
{
	cPhysics obstacle;
	D3DXVECTOR3 obsPos = *obj.GetPositionPtr();
	obstacle.SetPositionPtr(&D3DXVECTOR3(obsPos.x,0,obsPos.z));
	
	ObjVSObj(obj, obstacle);

	if (obj.GetVelocityScalar() < EPSILON_VELOCITY)
	{
		obj.SetIsActivate(false);
	}

	(*obj.GetPositionPtr()).y = 0.0f;
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