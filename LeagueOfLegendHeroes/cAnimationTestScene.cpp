#include "stdafx.h"
#include "cAnimationTestScene.h"
#include "cUnit.h"

cAnimationTestScene::cAnimationTestScene():
	m_pTestUnit(NULL)
{

}


cAnimationTestScene::~cAnimationTestScene()
{
	SAFE_DELETE(m_pTestUnit);
}

void cAnimationTestScene::Setup()
{
	g_pShaderManager->SetSkyBoxOn(false);

	m_pTestUnit = new cUnit;
	vector<ST_UNITLOADINFO> temp;
	
	////Alistar
	//temp.push_back({ STATE_IDLE, "unit/AlistarIdle.x" });
	//temp.push_back({ STATE_RUN, "unit/AlistarRun.x" });
	//temp.push_back({ STATE_ATTACK1, "unit/AlistarAttack1.x" });
	//temp.push_back({ STATE_ATTACK2, "unit/AlistarAttack2.x" });
	//temp.push_back({ STATE_ATTACK3, "unit/AlistarAttack3.x" });
	//temp.push_back({ STATE_SPELL1, "unit/AlistarSpell1.x"});
	//temp.push_back({ STATE_SPELL2, "unit/AlistarSpell2.x"});
	//temp.push_back({ STATE_DEAD, "unit/AlistarDead.x"});
	//	
	////Dari
	temp.push_back({ STATE_IDLE,	"unit/dariIdle.x" });
	temp.push_back({ STATE_RUN,		"unit/dariRun.x" });
	temp.push_back({ STATE_ATTACK1, "unit/dariAttack1.x" });
	temp.push_back({ STATE_ATTACK2, "unit/dariAttack2.x" });
	temp.push_back({ STATE_ATTACK3, "unit/dariAttack3.x" });
	temp.push_back({ STATE_SPELL1,	"unit/dariSpell1.x" });
	temp.push_back({ STATE_SPELL2,	"unit/dariSpell2.x" });
	temp.push_back({ STATE_DEAD,	"unit/dariDead.x" });

	//Hecarim
	/*temp.push_back({ STATE_IDLE,	"unit/hecarimIdle.x" });
	temp.push_back({ STATE_RUN,		"unit/hecarimRun.x" });
	temp.push_back({ STATE_ATTACK1, "unit/hecarimAttack1.x" });
	temp.push_back({ STATE_ATTACK2, "unit/hecarimAttack2.x" });
	temp.push_back({ STATE_ATTACK3, "unit/hecarimAttack3.x" });
	temp.push_back({ STATE_SPELL1,	"unit/hecarimSpell1.x" });
	temp.push_back({ STATE_SPELL2,	"unit/hecarimSpell2.x" });
	temp.push_back({ STATE_DEAD,	"unit/hecarimDead.x" });*/
	
	
	m_pTestUnit->Setup(temp, NULL);
	g_pCamera->SetTarget(m_pTestUnit->GetPosPtr());
	g_pShaderManager->SetTarget(g_pCamera->GetTarget());
}

void cAnimationTestScene::Update()
{
	if (g_pKeyManager->IsOnceKeyDown('1'))
	{
		m_pTestUnit->SetState(STATE_IDLE);
	}
	if (g_pKeyManager->IsOnceKeyDown('2'))
	{
		m_pTestUnit->SetState(STATE_RUN);
	}
	if (g_pKeyManager->IsOnceKeyDown('3'))
	{
		m_pTestUnit->SetState(STATE_ATTACK1);
	}
	if (g_pKeyManager->IsOnceKeyDown('4'))
	{
		m_pTestUnit->SetState(STATE_ATTACK2);
	}
	if (g_pKeyManager->IsOnceKeyDown('5'))
	{
		m_pTestUnit->SetState(STATE_ATTACK3);
	}
	if (g_pKeyManager->IsOnceKeyDown('6'))
	{
		m_pTestUnit->SetState(STATE_SPELL1);
	}
	if (g_pKeyManager->IsOnceKeyDown('7'))
	{
		m_pTestUnit->SetState(STATE_SPELL2);
	}
	if (g_pKeyManager->IsOnceKeyDown('8'))
	{
		m_pTestUnit->SetState(STATE_SPELL3);
	}
	if (g_pKeyManager->IsOnceKeyDown('9'))
	{
		m_pTestUnit->SetState(STATE_DEAD);
	}
	m_pTestUnit->Update();
}

void cAnimationTestScene::Render()
{
	m_pTestUnit->Render();
}

void cAnimationTestScene::UIRender()
{
}