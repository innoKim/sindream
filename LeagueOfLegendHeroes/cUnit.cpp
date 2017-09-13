#include "stdafx.h"
#include "cUnit.h"
#include "cSkinnedMesh.h"

cUnit::cUnit() :
	m_vPos(0, 0, 0),
	m_vDir(0, 0, -1)
{
}

cUnit::~cUnit()
{
	for each(auto p in m_mapStates)
	{
		SAFE_DELETE(p.second);
	}
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
}

void cUnit::Update()
{
	m_pCurState->Update();
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
