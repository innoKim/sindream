#include "stdafx.h"
#include "cPhysics.h"

cPhysics::cPhysics() :
	m_bIsActivate(false),
	m_fRestDuration(0.0f),
	m_pvPos(NULL),
	m_vAcceleration(0, 0, 0),
	m_vVelocity(0, 0, 0),
	m_vElasticFactor(0.7f),
	m_fRadius(0.0f),
	m_fMass(10000.0f)
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

	if(m_pvecTargets) ColisionWith(*m_pvecTargets);
	else ColisionWith(vector<cPhysics*>());

	if(m_bIsActivate) m_vAcceleration.y += PHYSICS_GRAVITY;
	
	if ((*m_pvPos).y>10.0f) m_vAcceleration.y += PHYSICS_GRAVITY;
	
	m_vVelocity *= 0.98f;

	m_vVelocity	+= m_vAcceleration;
	
	(*m_pvPos) += m_vVelocity;
}

bool cPhysics::ColisionWith(vector<cPhysics*> vecVersusObject)
{
	//���� ������ �浹�� ó���Ѵ�. ������ ������ �浹�ߴ� ���� �Ű�Ⱦ��� �ٷ� �˻��Ѵ�.
	if ((*m_pvPos).y<0) g_pCollisionCalculator->ObjVSObstacle(*this);

	//�ڽ��� �̹� �����ӿ� �ѹ��̶� �浹������ �浹���꿡�� ������.
	if (m_fRestDuration < g_pTimeManager->GetElapsedTime()) return false;

	//��� ����������Ʈ��� �浹�� �����Ѵ�.
	if (!vecVersusObject.empty())
	{
		for (int i = 0; i < vecVersusObject.size(); i++)
		{
			//���ڽ��� �浹 ���Ѵ�.
			if (vecVersusObject[i] == this)	continue;
			
			//�浹 ����γ��� �̹� �����ӿ� �ѹ��̶� �浹������ �浹���꿡�� ������.
			if (vecVersusObject[i]->GetRestDuration() < g_pTimeManager->GetElapsedTime()) continue;

			//�浹 ������ �ȵ����� �浹 ���Ѵ�.
			float distance = D3DXVec3Length(&((*vecVersusObject[i]->GetPositionPtr()) - (*m_pvPos)));

			if (distance > (vecVersusObject[i]->GetRadius()+m_fRadius)) continue;
		
			g_pCollisionCalculator->ObjVSObj(*this, *vecVersusObject[i],false);
		}
	}
}

