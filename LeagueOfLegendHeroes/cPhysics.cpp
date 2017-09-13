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
