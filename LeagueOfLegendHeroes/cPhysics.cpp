#include "stdafx.h"
#include "cPhysics.h"

cPhysics::cPhysics() :
	m_bIsActivate(true)
{
}

cPhysics::~cPhysics()
{
}

void cPhysics::Setup()
{
}

void cPhysics::Update()
{
	m_fRestDuration += g_pTimeManager->GetElapsedTime();

	ColisionWith(vector<cPhysics*>());

	if (!m_bIsActivate) return;

	m_vAcceleration.y += PHYSICS_GRAVITY;
	m_vVelocity		+= m_vAcceleration;
	(*m_pvPos)		+= m_vVelocity;
}

bool cPhysics::ColisionWith(vector<cPhysics*> vecVersusObject)
{
	if ((*m_pvPos).y<m_fRadius) g_pCollisionCalculator->ObjVSObstacle(*this);

	if (m_fRestDuration < g_pTimeManager->GetElapsedTime()) return false;

	if (!vecVersusObject.empty())
	{
		for (int i = 0; i < vecVersusObject.size(); i++)
		{
			if (vecVersusObject[i]->GetRestDuration() < g_pTimeManager->GetElapsedTime()) continue;

			if (D3DXVec3Length(&((*vecVersusObject[i]->GetPositionPtr()) - (*m_pvPos))) > (vecVersusObject[i]->GetRadius()+m_fRadius)) continue;
		
			g_pCollisionCalculator->ObjVSObj(*this, *vecVersusObject[i]);
		}
	}
}

