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
#include "cGridPlane.h"

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
	m_nIndexBuilding(0),
	m_pGrid(NULL),
	m_nElasticFactor(3)
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

	SAFE_DELETE(m_pGrid);
}

void cPhysicsScene::Setup()
{
	//m_pMap = new cMap;
	//m_pMap->LoadMap("map/", "room.obj");
	//m_pMap->LoadSur("LoL/room_surface.obj");
	//
	//cBuildingLoader buildingLoader;
	//buildingLoader.LoadBuilding("building.txt", m_pMap, m_vecBuilding);

	m_pGrid = new cGridPlane();
	m_pGrid->Setup(100, 50);

	m_pPlayer = new cPlayer;
	vector<ST_UNITLOADINFO> temp;
	temp.push_back({ STATE_IDLE, "unit/AlistarIdle.x"});
	temp.push_back({ STATE_RUN, "unit/AlistarRun.x"});
	temp.push_back({ STATE_SPELL1, "unit/AlistarSpell1.x",ST_CallbackInfo(0.0f,AlistarSpell1CallBack,m_pPlayer),ST_CallbackInfo(0.5f,AlistarSpell1CallBack2,m_pPlayer)});
	temp.push_back({ STATE_SPELL2, "unit/AlistarSpell2.x",ST_CallbackInfo(0.0f,AlistarSpell2CallBack,m_pPlayer),ST_CallbackInfo(0.5f,AlistarSpell2CallBack2,m_pPlayer)});
	m_pPlayer->Setup(temp, m_pMap);
	m_pPlayer->SetPosition(D3DXVECTOR3(0, 0, 0));
	g_pCamera->SetTarget(m_pPlayer->GetPosPtr());
	g_pShaderManager->SetTarget(g_pCamera->GetTarget());
	g_pShaderManager->SetSkyBoxOn(false);

	//물리관련
	m_pPlayer->GetPhysics()->SetIsActivate(false);
	///요기까지 물리

	SetLight();
}

void cPhysicsScene::Update()
{
	m_pPlayer->Update();
	if(m_pMap) m_pPlayer->SetPosY(m_pMap->GetHeight(m_pPlayer->GetPosition()));

	for each(auto p in m_vecBuilding)
	{
		p->Update();
	}

	for each(auto p in m_vecEnemy)
	{
		p->Update(m_pPlayer->GetPosition());
	}

	if (g_pKeyManager->IsOnceKeyDown(VK_SPACE))
	{
		m_vecEnemy[0]->GetPhysics()->SetVelocity(D3DXVECTOR3(500, 500, 0));
	}

	if (g_pKeyManager->IsOnceKeyDown(VK_LBUTTON))
	{
		RayInfo newRay = RayToScreenPoint(g_ptMouse.x, g_ptMouse.y);
		
		cEnemy* enemy = new cEnemy;
		enemy->Setup(cEnemy::ENEMYTYPE_MELEE, m_pMap);
		enemy->SetPosition(D3DXVECTOR3(newRay.pos));
		enemy->SetDirection(newRay.dir);
		enemy->GetPhysics()->SetVelocity(newRay.dir*THROW_SPD);
		enemy->GetPhysics()->SetElastic(m_nElasticFactor*0.1f);
		m_vecEnemy.push_back(enemy);
	}

	if (g_pKeyManager->IsOnceKeyDown(VK_UP))
	{
		if (m_nElasticFactor <20)
		{
			m_nElasticFactor ++;
		}
		for each (auto p in *g_pPhysicsManager->GetSet())
		{
			p->SetElastic(m_nElasticFactor*0.1f);
		}
	}
	if (g_pKeyManager->IsOnceKeyDown(VK_DOWN))
	{
		if (m_nElasticFactor >0 )
		{
			m_nElasticFactor --;
		}
		for each (auto p in *g_pPhysicsManager->GetSet())
		{
			p->SetElastic(m_nElasticFactor*0.1f);
		}
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

void cPhysicsScene::UIRender()
{
	m_pGrid->Render();

	char str[128];
	sprintf(str, "Elastic Factor : %.2f", m_nElasticFactor*0.1f);
	g_pDebug->Print(str, 50, 50);
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