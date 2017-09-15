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
	//���� ������ �浹�� ó���Ѵ�. ������ ������ �浹�ߴ� ���� �Ű�Ⱦ��� �ٷ� �˻��Ѵ�.
	if ((*m_pvPos).y<0) g_pCollisionCalculator->ObjVSObstacle(*this);

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

