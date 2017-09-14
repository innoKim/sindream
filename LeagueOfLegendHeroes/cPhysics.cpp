#include "stdafx.h"
#include "cPhysics.h"


cPhysics::cPhysics()
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
	m_vVelocity		+= m_vAcceleration;
	(*m_pvPos)		+= m_vVelocity;

	m_fRestDuration += g_pTimeManager->GetElapsedTime();
}

bool cPhysics::ColisionWith(vector<cPhysics*> vecVersusObject)
{
	for (int i = 0; i < vecVersusObject.size(); i++)
	{
		if (m_fRestDuration > 0 || vecVersusObject[i]->GetRestDuration()>0) return true;

		SphereVsSphere(this, vecVersusObject[i]);
	}	

	return true;
}

void cPhysics::SphereVsSphere(cPhysics * Object1, cPhysics * Object2)
{
	D3DXVECTOR3 distanceVector = (*Object1->GetPositionPtr()) - (*Object2->GetPositionPtr());
	float dist = D3DXVec3LengthSq(&distanceVector);

	if (((Object1->GetRadius() + Object1->GetRadius()) / 2)*((Object1->GetRadius() + Object1->GetRadius()) / 2) > dist) return;
	


	Object1->SetRestDuration(0.0f);
	Object2->SetRestDuration(0.0f);
}

void cPhysics::SphereVsGround()
{
	
}
