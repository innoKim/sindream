#include "stdafx.h"
#include "cPhysicsScene.h"

#include "cUnit.h"

#include "cPlayer.h"
#include "cEnemy.h"

#include "cPlane.h"

void cPhysicsScene::SetLight()
{
	m_light.Type = D3DLIGHT_DIRECTIONAL;
	m_light.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_light.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_light.Direction = D3DXVECTOR3(-1.0f, -0.5f, 1.0f);

	g_pD3DDevice->SetLight(0, &m_light);
	g_pD3DDevice->LightEnable(0, true);

	g_pD3DDevice->SetRenderState(D3DRS_SPECULARENABLE, true);
}

cPhysicsScene::cPhysicsScene():
	m_pPlayer(NULL),
	m_pPlane(NULL)
{
	for each(auto p in m_vecEnemy)
	{
		SAFE_DELETE(p);
	}
}

cPhysicsScene::~cPhysicsScene()
{
	SAFE_DELETE(m_pPlayer);
	SAFE_DELETE(m_pPlane);
}

void cPhysicsScene::Setup()
{
	m_pPlayer = new cPlayer;
	vector<ST_UNITLOADINFO> temp;
	temp.push_back({ STATE_IDLE, "unit/AlistarIdle.x" ,NULL,NULL });
	temp.push_back({ STATE_RUN, "unit/AlistarRun.x",NULL,NULL });
	temp.push_back({ STATE_SPELL1, "unit/AlistarSpell1.x",AlistarSpell1CallBack,m_pPlayer });
	temp.push_back({ STATE_SPELL2, "unit/AlistarSpell2.x",AlistarSpell2CallBack,m_pPlayer });
	m_pPlayer->Setup(temp);
	m_pPlayer->SetPosition(D3DXVECTOR3(0, 300, 0));
	g_pCamera->SetTarget(m_pPlayer->GetPosPtr());
	g_pShaderManager->SetTarget(g_pCamera->GetTarget());

	m_pPlane = new cPlane;
	m_pPlane->Setup(5000);

	for (int i = 0; i < 3; i++)
	{
		cUnit* enemy = new cEnemy;
		vector<ST_UNITLOADINFO> temp;
		temp.push_back({ STATE_IDLE, "unit/PoroIdle.x" ,NULL,NULL });
		enemy->Setup(temp);
		enemy->SetPosition(D3DXVECTOR3(i * 100, 300 , 0));
		m_vecEnemy.push_back(enemy);
	}
	
	SetLight();
}

void cPhysicsScene::Update()
{
	m_pPlayer->Update();

	for each(auto p in m_vecEnemy)
	{
		p->Update();
	}
}

void cPhysicsScene::Render()
{
	m_pPlayer->Render();

	for each(auto p in m_vecEnemy)
	{
		p->Render();
	}
}

void cPhysicsScene::AlistarSpell1CallBack(void *CallBackObj)
{
	cUnit* Alistar = (cUnit*)CallBackObj;

	Alistar->SetState(STATE_IDLE);
}

void cPhysicsScene::AlistarSpell2CallBack(void * CallBackObj)
{
	cUnit* Alistar = (cUnit*)CallBackObj;

	Alistar->SetState(STATE_IDLE);
}
