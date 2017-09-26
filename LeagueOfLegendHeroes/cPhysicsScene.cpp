#include "stdafx.h"
#include "cPhysicsScene.h"
#include "cBuilding.h"
#include "cUnit.h"
#include "cPlayer.h"
#include "cEnemy.h"
#include "cPhysics.h"
#include "cRigidbody.h"
#include "cMap.h"
#include "cBuildingLoader.h"

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
	m_pMap(NULL),
	m_bEditOn(false),
	m_pCurrentBuilding(NULL),
	m_nIndexBuilding(0)
{
	
}

cPhysicsScene::~cPhysicsScene()
{
	SAFE_DELETE(m_pPlayer);
	SAFE_DELETE(m_pMap);

	for each(auto p in m_vecBuilding)
	{
		SAFE_DELETE(p);
	}

	for each(auto p in m_vecEnemy)
	{
		SAFE_DELETE(p);
	}
}

void cPhysicsScene::Setup()
{
	m_pMap = new cMap;
	m_pMap->LoadMap("map/", "room.obj");
	m_pMap->LoadSur("LoL/room_surface.obj");

	cBuildingLoader buildingLoader;
	buildingLoader.LoadBuilding(m_pMap, m_vecBuilding);

	m_pPlayer = new cPlayer;
	vector<ST_UNITLOADINFO> temp;
	temp.push_back({ STATE_IDLE, "unit/AlistarIdle.x"});
	temp.push_back({ STATE_RUN, "unit/AlistarRun.x"});
	temp.push_back({ STATE_SPELL1, "unit/AlistarSpell1.x",ST_CallbackInfo(0.0f,AlistarSpell1CallBack,m_pPlayer),ST_CallbackInfo(0.5f,AlistarSpell1CallBack2,m_pPlayer)});
	temp.push_back({ STATE_SPELL2, "unit/AlistarSpell2.x",ST_CallbackInfo(0.0f,AlistarSpell2CallBack,m_pPlayer),ST_CallbackInfo(0.5f,AlistarSpell2CallBack2,m_pPlayer)});
	m_pPlayer->Setup(temp, m_pMap);
	m_pPlayer->SetPosition(D3DXVECTOR3(1000, 0, 1000));
	g_pCamera->SetTarget(m_pPlayer->GetPosPtr());
	g_pShaderManager->SetTarget(g_pCamera->GetTarget());

	for (int i = 0; i < 3; i++)
	{
		cUnit* enemy = new cEnemy;
		vector<ST_UNITLOADINFO> temp;
		temp.push_back({ STATE_IDLE, "unit/PoroIdle.x" ,NULL,NULL });
		enemy->Setup(temp, m_pMap);
		enemy->SetPosition(D3DXVECTOR3(1000+i * 100, 100 , 1000 + i * 10));
		m_vecEnemy.push_back(enemy);
	}
	
	//물리관련
	m_pPlayer->GetPhysics()->SetIsActivate(false);
	///요기까지 물리

	SetLight();
}

void cPhysicsScene::Update()
{
	m_pPlayer->Update();
	m_pPlayer->SetPosY(m_pMap->GetHeight(m_pPlayer->GetPosition()));

	for each(auto p in m_vecBuilding)
	{
		p->Update();
	}

	for each(auto p in m_vecEnemy)
	{
		p->Update();
	}

	if (g_pKeyManager->IsOnceKeyDown(VK_SPACE))
	{
		m_vecEnemy[0]->GetPhysics()->SetVelocity(D3DXVECTOR3(500, 500, 0));
	}
}

void cPhysicsScene::Render()
{
	m_pPlayer->Render();

	for each(auto p in m_vecEnemy)
	{
		p->Render();
	}

	for each(auto p in m_vecBuilding)
	{
		p->Render();
	}
}

void cPhysicsScene::AlistarSpell1CallBack(void *CallBackObj)
{
	cUnit* Alistar = (cUnit*)CallBackObj;
	
	Alistar->SetState(STATE_IDLE);
}

void cPhysicsScene::AlistarSpell2CallBack2(void *CallBackObj)
{
	cUnit* Alistar = (cUnit*)CallBackObj;

	set<cPhysics*> targets = g_pPhysicsManager->GetTargets(Alistar->GetPosition(), 100);

	D3DXVECTOR3 dir = Alistar->GetDirection();

	for each (auto target in targets)
	{
		target->SetAcceleration(D3DXVECTOR3(0, 0, 0));
		target->SetVelocity(target->GetVelocity() + D3DXVECTOR3(dir.x*500, 100, dir.z * 500));
		target->SetAngularVelocity(D3DXVECTOR3(rand()%20-10, rand() % 20 - 10, rand() % 20 - 10));
	}
}


void cPhysicsScene::AlistarSpell1CallBack2(void *CallBackObj)
{
	cUnit* Alistar = (cUnit*)CallBackObj;

	set<cPhysics*> targets = g_pPhysicsManager->GetTargets(Alistar->GetPosition(), 100);

	for each (auto target in targets)
	{
		target->SetAcceleration(D3DXVECTOR3(0, 0, 0));
		target->SetVelocity(target->GetVelocity() + D3DXVECTOR3(rand() % 100 - 50, 700, rand() % 100 - 50));
		target->SetAngularVelocity(D3DXVECTOR3(rand() % 20 - 10, rand() % 20 - 10, rand() % 20 - 10));
	}
}

void cPhysicsScene::AlistarSpell2CallBack(void * CallBackObj)
{
	cUnit* Alistar = (cUnit*)CallBackObj;

	Alistar->SetState(STATE_IDLE);
}

D3DXVECTOR3 cPhysicsScene::playerPos()
{
	return m_pPlayer->GetPosition();
}
