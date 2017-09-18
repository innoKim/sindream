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
	m_vRotationInertia(38.5f, 38.5f, 38.5f),
	m_vTorque(0, 0, 0)
{
	g_pPhysicsManager->AddRigidbodyToSet(this);
}

cRigidbody::~cRigidbody()
{
	g_pPhysicsManager->RemoveRigidbodyFromSet(this);
}

void cRigidbody::Update(float deltaTime)
{
	m_fRestDuration += g_pTimeManager->GetElapsedTime();

	if (!m_bIsActivate) return;

	/*if ((*m_pvPos).y>10.0f) m_vAcceleration.y += PHYSICS_GRAVITY;

	else if ((*m_pvPos).y < 5.0f)
	{
		m_vVelocity *= 0.9f;
	}

	m_vVelocity *= 0.98f;*/

	//선형 운동 계산
	m_vAcceleration += m_stForce.force / m_fMass; //+D3DXVECTOR3(0,PHYSICS_GRAVITY,0);
	m_vVelocity += m_vAcceleration*deltaTime;
	*m_pvPos += m_vVelocity*deltaTime;

	//회전 운동 계산
	D3DXVec3Cross(&m_vTorque,&m_stForce.pos, &m_stForce.force);
	
	m_vAngularAcceleration.x = m_vTorque.x / m_vRotationInertia.x;
	m_vAngularAcceleration.y = m_vTorque.y / m_vRotationInertia.y;
	m_vAngularAcceleration.z = m_vTorque.z / m_vRotationInertia.z;

	m_vAngularVelocity += m_vAngularAcceleration*deltaTime;

	D3DXMATRIXA16 rotx, roty, rotz;
	D3DXMatrixRotationX(&rotx,m_vAngularVelocity.x*deltaTime);
	D3DXMatrixRotationY(&roty,m_vAngularVelocity.y*deltaTime);
	D3DXMatrixRotationZ(&rotz,m_vAngularVelocity.z*deltaTime);
	D3DXVec3TransformNormal(m_pvDir, m_pvDir, &(rotx*roty*rotz));
}
