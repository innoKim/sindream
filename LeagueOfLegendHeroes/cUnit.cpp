#include "stdafx.h"
#include "cUnit.h"
#include "cSkinnedMesh.h"
#include "cPhysics.h"
#include "cMap.h"
#include "cAStar.h"

void cUnit::Destroy()
{
	SAFE_DELETE(m_pAStar);

	for each(auto p in m_mapStates)
	{
		SAFE_DELETE(p.second);
	}
	SAFE_DELETE(m_pPhysics);
}

cUnit::cUnit() :
	m_vPos(0, 0, 0),
	m_vDir(0, 0, -1),
	m_pPhysics(NULL),
	m_pMap(NULL),
	m_pAStar(NULL)
{
}

cUnit::~cUnit()
{
}

void cUnit::Setup(vector<ST_UNITLOADINFO> statesVector, cMap* mapPtr)
{
	for (int i = 0; i < statesVector.size(); i++)
	{
		cSkinnedMesh* newState = new cSkinnedMesh;
		newState->Setup(statesVector[i].fileName);
		m_mapStates[statesVector[i].state] = newState;
		newState->SetCallBack(statesVector[i].cbInfo1.pfnCallBack, statesVector[i].cbInfo1.pCallBackObj, statesVector[i].cbInfo1.fTime);
		newState->SetCallBack(statesVector[i].cbInfo2.pfnCallBack, statesVector[i].cbInfo2.pCallBackObj, statesVector[i].cbInfo2.fTime);
	}
	SetState(STATE_IDLE);

	m_pMap = mapPtr;

	m_pAStarGrid = m_pMap->GetGrid();

	m_pAStar = new cAStar;
	m_pAStar->Setup(m_pAStarGrid);

	/*m_pRigidbody = new cRigidbody;
	m_pRigidbody->SetPositionPtr(&m_vPos);
	m_pRigidbody->SetDirectionPtr(&m_vDir);
	m_pRigidbody->SetRadius(15.0f);*/
	
	m_pPhysics = new cPhysics;
	m_pPhysics->SetPositionPtr(&m_vPos);
	m_pPhysics->SetDirectionPtr(&m_vDir);
	m_pPhysics->SetRadius(15.0f);
	m_pPhysics->Setup();
	m_pPhysics->SetMapPtr(mapPtr);
}

void cUnit::Update()
{
	m_pCurState->Update();
	m_pPhysics->Update(g_pTimeManager->GetElapsedTime());
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