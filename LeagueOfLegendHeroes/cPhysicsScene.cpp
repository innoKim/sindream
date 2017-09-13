#include "stdafx.h"
#include "cPhysicsScene.h"
#include "cUnit.h"
#include "cPlayer.h"


cPhysicsScene::cPhysicsScene():
	m_pPlayer(NULL)
{
}

cPhysicsScene::~cPhysicsScene()
{
	SAFE_DELETE(m_pPlayer);
}

void cPhysicsScene::Setup()
{
	m_pPlayer = new cPlayer;

	vector<ST_UNITLOADINFO> temp;
	temp.push_back({ STATE_IDLE, "unit/AlistarIdle.x" ,NULL,NULL });
	temp.push_back({ STATE_RUN, "unit/AlistarRun.x",NULL,NULL });
	temp.push_back({ STATE_SPELL1, "unit/AlistarSpell1.x",AlistarSpell1CallBack,m_pPlayer });

	m_pPlayer->Setup(temp);
}

void cPhysicsScene::Update()
{
	m_pPlayer->Update();
}

void cPhysicsScene::Render()
{
	m_pPlayer->Render();
}

void cPhysicsScene::AlistarSpell1CallBack(void *CallBackObj)
{
	cUnit* Alistar = (cUnit*)CallBackObj;

	Alistar->SetState(STATE_IDLE);
}
