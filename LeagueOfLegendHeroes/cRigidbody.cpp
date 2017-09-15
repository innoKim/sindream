#include "stdafx.h"
#include "cRigidbody.h"


cRigidbody::cRigidbody():
	m_pvPos(NULL),
	m_fRadius(0.0f),
	m_vElasticFactor(0.5f),
	m_bIsActivate(true),
	m_fRestDuration(0.0f),
	m_fMass(1.0f),
	m_vVelocity(0,0,0),
	m_vAcceleration(0,0,0),
	m_vAngularVelocity(0,0,0),
	m_vAngularAcceleration(0, 0, 0),
	m_vRotationInertia(0, 0, 0),
	m_vTorque(0, 0, 0)
{
}

cRigidbody::~cRigidbody()
{
}

void cRigidbody::Update()
{
	//선형 운동 계산
	m_vAcceleration = m_stForce.force / m_fMass;
	m_vVelocity += m_vAcceleration;
	*m_pvPos += m_vVelocity;

	//회전 운동 계산
	D3DXVec3Cross(&m_vTorque,&m_stForce.pos, &m_stForce.force);
	
	m_vAngularAcceleration.x = m_vTorque.x / m_vRotationInertia.x;
	m_vAngularAcceleration.y = m_vTorque.y / m_vRotationInertia.y;
	m_vAngularAcceleration.z = m_vTorque.z / m_vRotationInertia.z;

	m_vAngularVelocity += m_vAngularAcceleration;

	D3DXMATRIXA16 rotx, roty, rotz;
	D3DXMatrixRotationX(&rotx,m_vAngularVelocity.x);
	D3DXMatrixRotationY(&roty,m_vAngularVelocity.y);
	D3DXMatrixRotationZ(&rotz,m_vAngularVelocity.z);
	D3DXVec3TransformNormal(m_pvDir, m_pvDir, &(rotx, roty, rotz));
}
