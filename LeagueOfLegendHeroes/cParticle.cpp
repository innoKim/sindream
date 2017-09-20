#include "stdafx.h"
#include "cParticle.h"


cParticle::cParticle():
	m_pvPosition(NULL),
	m_fLifeTime(0.0f),
	m_fCurTime(0.0f),
	m_vAcceleration(0,0,0),
	m_vVelocity(0,0,0),
	m_dStartColor(1,1,1,1),
	m_dEndColor(0,0,0,0),
	m_fDragVelocity(0.0f)
{
}


cParticle::~cParticle()
{
}

void cParticle::Setup(D3DXVECTOR3* PositionPtr, D3DXVECTOR3 Velocity, D3DXVECTOR3 Acceleration, float LifeTime, D3DXCOLOR StartColor, D3DXCOLOR EndColor, float DragVelocity)
{
	m_pvPosition		= PositionPtr;
	m_vAcceleration		= Velocity;
	m_vVelocity			= Acceleration;
	m_fLifeTime			= LifeTime;
	m_dStartColor		= StartColor;
	m_dEndColor			= EndColor;
	m_fDragVelocity		= DragVelocity;
}

void cParticle::Update()
{
	m_fCurTime += g_pTimeManager->GetElapsedTime();

	float VelocityScalar = D3DXVec3Length(&m_vVelocity);
	
	if (m_fDragVelocity > FLT_EPSILON)
	{
		if (VelocityScalar<m_fDragVelocity)	m_vVelocity += m_vAcceleration;
		else m_vVelocity += m_vVelocity*(VelocityScalar - m_fDragVelocity)*0.1f;
	}
	
	(*m_pvPosition) += m_vVelocity;
}

D3DXCOLOR cParticle::GetCurColor()
{
	D3DXCOLOR curColor;

	float t = m_fCurTime / m_fLifeTime;
	curColor.a = m_dStartColor.a*(1	- t) + m_dEndColor.a*t;
	curColor.r = m_dStartColor.r*(1 - t) + m_dEndColor.r*t;
	curColor.g = m_dStartColor.g*(1 - t) + m_dEndColor.g*t;
	curColor.b = m_dStartColor.b*(1 - t) + m_dEndColor.b*t;
	
	return curColor;
}
