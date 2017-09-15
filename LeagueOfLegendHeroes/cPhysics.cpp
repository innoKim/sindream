#include "stdafx.h"
#include "cPhysics.h"

cPhysics::cPhysics() :
	m_bIsActivate(true),
	m_fRestDuration(0.0f),
	m_pvPos(NULL),
	m_vAcceleration(0, 0, 0),
	m_vVelocity(0, 0, 0),
	m_vElasticFactor(0.5f),
	m_fRadius(0.0f),
	m_fMass(1.0f)
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

void cPhysics::Update()
{
	m_fRestDuration += g_pTimeManager->GetElapsedTime();

	ColisionWith(*g_pPhysicsManager->GetSet());


	if (!m_bIsActivate) return;
	if ((*m_pvPos).y>10.0f) m_vAcceleration.y += PHYSICS_GRAVITY;

	else if ((*m_pvPos).y < 5.0f)
	{
		m_vVelocity *= 0.9f;
	}

	m_vVelocity *= 0.98f;
	m_vVelocity	+= m_vAcceleration;
	(*m_pvPos) += m_vVelocity;
}

bool cPhysics::ColisionWith(set<cPhysics*> setVersusObject)
{
	//먼저 지형과 충돌을 처리한다. 지형은 이전에 충돌했던 말던 신경안쓰고 바로 검사한다.
	if ((*m_pvPos).y<0) g_pCollisionCalculator->ObjVSObstacle(*this);

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

