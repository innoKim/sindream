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
	//���� ������ �浹�� ó���Ѵ�. ������ ������ �浹�ߴ� ���� �Ű�Ⱦ��� �ٷ� �˻��Ѵ�.
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
	

	//�ڽ��� �̹� �����ӿ� �ѹ��̶� �浹������ �浹���꿡�� ������.
	if (m_fRestDuration < g_pTimeManager->GetElapsedTime()) return false;

	//��� ����������Ʈ��� �浹�� �����Ѵ�.
	if (!setVersusObject.empty())
	{
		for each (auto p in setVersusObject)
		{
			//���ڽ��� �浹 ���Ѵ�.
			if (p == this)	continue;

			//�浹 ����γ��� �̹� �����ӿ� �ѹ��̶� �浹������ �浹���꿡�� ������.
			if (p->GetRestDuration() < g_pTimeManager->GetElapsedTime()) continue;

			//�浹 ������ �ȵ����� �浹 ���Ѵ�.
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

