#include "stdafx.h"
#include "cUnit.h"
#include "cSkinnedMesh.h"
#include "cPhysics.h"

void cUnit::Destroy()
{
	for each(auto p in m_mapStates)
	{
		SAFE_DELETE(p.second);
	}
	SAFE_DELETE(m_pPhysics);
}

cUnit::cUnit() :
	m_vPos(0, 0, 0),
	m_vDir(0, 0, -1),
	m_pPhysics(NULL)
{
}

cUnit::~cUnit()
{
}

void cUnit::Setup(vector<ST_UNITLOADINFO> statesVector)
{
	for (int i = 0; i < statesVector.size(); i++)
	{
		cSkinnedMesh* newState = new cSkinnedMesh;
		newState->Setup(statesVector[i].fileName);
		m_mapStates[statesVector[i].state] = newState;
		newState->SetCallBack(statesVector[i].callbackFunc, statesVector[i].callbackObj);
	}
	SetState(STATE_IDLE);

	m_pPhysics = new cPhysics;
	m_pPhysics->SetPositionPtr(&m_vPos);
	m_pPhysics->SetRadius(15.0f);
	m_pPhysics->Setup();
}

void cUnit::Update()
{
	m_pCurState->Update();
	m_pPhysics->Update();
}

void cUnit::Render()
{
	m_pCurState->Render(&m_vPos, &m_vDir);
}

void cUnit::SetState(eSTATE state)
{
	m_pCurState = m_mapStates[state];
	m_eStateNum = state;
}