#include "stdafx.h"
#include "cPhysics.h"
#include "cMap.h"

cPhysics::cPhysics() :
	m_bIsActivate(true),
	m_fRestDuration(0.0f),
	m_pvPos(NULL),
	m_vAcceleration(0, 0, 0),
	m_vVelocity(0, 0, 0),
	m_vElasticFactor(0.5f),
	m_fRadius(0.0f),
	m_fMass(1.0f),
	m_pvDir(NULL),
	m_vAngularVelocity(0, 0, 0)
{
	g_pPhysicsManager->AddToSet(this);
}

cPhysics::~cPhysics()
{
	g_pPhysicsManager->RemoveFromSet(this);
}

void cPhysics::Setup()
{
}

void cPhysics::Update(float deltaTime)
{
	m_fRestDuration += deltaTime;

	ColisionWith(*g_pPhysicsManager->GetSet());
	
	if (!m_bIsActivate)
	{
		m_vVelocity = D3DXVECTOR3(0, 0, 0);
		m_vAcceleration = D3DXVECTOR3(0, 0, 0);
		return;
	}
	D3DXVECTOR3 Acc = m_vAcceleration;
	
	if ((*m_pvPos).y>  Height() + 1.0f) Acc.y += PHYSICS_GRAVITY;
	
	else if ((*m_pvPos).y < Height() + 1.0f)
	{
		m_vVelocity *= 0.9f;
		m_vAngularVelocity *= 0.5f;
	}

	m_vVelocity *= 0.99f;
	
	m_vVelocity	+= Acc*deltaTime;
	
	(*m_pvPos) += m_vVelocity*deltaTime;

	D3DXMATRIXA16 rotx, roty, rotz;
	D3DXMatrixRotationX(&rotx, m_vAngularVelocity.x*deltaTime);
	D3DXMatrixRotationY(&roty, m_vAngularVelocity.y*deltaTime);
	D3DXMatrixRotationZ(&rotz, m_vAngularVelocity.z*deltaTime);
	D3DXVec3TransformNormal(m_pvDir, m_pvDir, &(rotx*roty*rotz));
}

bool cPhysics::ColisionWith(set<cPhysics*> setVersusObject)
{
	//먼저 지형과 충돌을 처리한다. 지형은 이전에 충돌했던 말던 신경안쓰고 바로 검사한다.
	if ((*m_pvPos).y<Height()) g_pCollisionCalculator->ObjVSObstacle(*this);
	if (GetVelocityScalar() > FLT_EPSILON)
	{
		D3DXVECTOR3 dir = m_vVelocity;
		D3DXVec3Normalize(&dir, &dir);
		if (m_pMap->GetHeight((*m_pvPos+ dir)) < FLT_EPSILON)
		{
			m_vVelocity.x *= -1;
			m_vVelocity.z *= -1;
			//g_pCollisionCalculator->ObjVSWall(*this);
		}
	}
	

	//자신이 이번 프레임에 한번이라도 충돌했으면 충돌연산에서 빠진다.
	if (m_fRestDuration < g_pTimeManager->GetElapsedTime()) return false;

	//모든 물리오브젝트들과 충돌을 연산한다.
	if (!setVersusObject.empty())
	{
		for each (auto p in setVersusObject)
		{
			//나자신은 충돌 안한다.
			if (p == this)	continue;

			//충돌 대상인놈이 이번 프레임에 한번이라도 충돌했으면 충돌연산에서 빠진다.
			if (p->GetRestDuration() < g_pTimeManager->GetElapsedTime()) continue;

			//충돌 범위에 안들어오면 충돌 안한다.
			float distance = D3DXVec3Length(&((*p->GetPositionPtr()) - (*m_pvPos)));

			if (distance > (p->GetRadius() + m_fRadius)) continue;

			g_pCollisionCalculator->ObjVSObj(*this, *p, false);
		}
	}
}

float cPhysics::Height()
{
	return (m_pMap) ? m_pMap->GetHeight(*m_pvPos) : 10.0f;
}

