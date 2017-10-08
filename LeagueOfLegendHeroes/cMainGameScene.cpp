#include "stdafx.h"
#include "cMainGameScene.h"
#include "cBuilding.h"
#include "cUnit.h"
#include "cPlayer.h"
#include "cPhysics.h"
#include "cMap.h"
#include "cBuildingLoader.h"
#include "cUIObject.h"
#include "cUIButton.h"
#include "cUIText.h"
#include "cAlphablending.h"
#include "cAStar.h"
#include "cAStarGrid.h"
#include "cEnemy.h"

cMainGameScene::cMainGameScene()
	: m_pPlayer(NULL)
	, m_pMap(NULL)
	, m_pSprite(NULL)
	, m_bEditOn(false)
	, m_pCurrentBuilding(NULL)
	, m_nIndexBuilding(0)
{
}


cMainGameScene::~cMainGameScene()
{
	SAFE_DELETE(m_pPlayer);
	SAFE_DELETE(m_pMap);

	m_pCurrentBuilding = NULL;

	SAFE_RELEASE(m_pSprite);

	for each (auto p in m_vecBuilding)
	{
		SAFE_DELETE(p);
	}

	for each(auto p in m_vecEnemy)
	{
		SAFE_DELETE(p);
	}
}

void cMainGameScene::Setup()
{
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);

	m_pMap = new cMap;
	m_pMap->LoadMap("map/", "room.obj");
	m_pMap->LoadSur("LoL/room_surface.obj");

	LoadBuilding("Buildings.txt");

	m_pPlayer = new cPlayer;
	vector<ST_UNITLOADINFO> temp;
	temp.push_back({ STATE_IDLE, "unit/AlistarIdle.x" });
	temp.push_back({ STATE_RUN, "unit/AlistarRun.x",ST_CallbackInfo(0.0f,AlistarWalkCallBack,m_pPlayer) });
	temp.push_back({ STATE_SPELL1, "unit/AlistarSpell1.x",ST_CallbackInfo(0.0f,AlistarSpell1CallBack,m_pPlayer),ST_CallbackInfo(0.5f,AlistarSpell1CallBack2,m_pPlayer) });
	temp.push_back({ STATE_SPELL2, "unit/AlistarSpell2.x",ST_CallbackInfo(0.0f,AlistarSpell2CallBack,m_pPlayer),ST_CallbackInfo(0.5f,AlistarSpell2CallBack2,m_pPlayer) });
	m_pPlayer->Setup(temp, m_pMap);
	m_pPlayer->SetPosition(D3DXVECTOR3(1000, 0, 1000));
	g_pCamera->SetTarget(m_pPlayer->GetPosPtr());
	g_pShaderManager->SetTarget(g_pCamera->GetTarget());

	g_pAlphablending->AddAlphablending("AlistarQ", "AlistarQgroundcrack.dds", m_pPlayer->GetPosition(), D3DXVECTOR3(0, 1, 0), 1.0f, 150, true, false);

	for (int i = 0; i < 3; i++)
	{
		cEnemy* enemy = new cEnemy;
		enemy->Setup(cEnemy::ENEMYTYPE_MELEE, m_pMap);
		enemy->SetPosition(D3DXVECTOR3(1500 + i * 100, 100, 1000 + i * 10));
		m_vecEnemy.push_back(enemy);
	}

	//물리관련
	m_pPlayer->GetPhysics()->SetIsActivate(false);
}

void cMainGameScene::Update()
{
	m_pPlayer->Update();
	m_pPlayer->SetPosY(m_pMap->GetHeight(m_pPlayer->GetPosition()));

	for each (auto p in m_vecUIObject)
	{
		p->Update();
	}

	for each(auto p in m_vecBuilding)
	{
		p->Update();
	}

	for each(auto p in m_vecEnemy)
	{
		p->Update(m_pPlayer->GetPosition());
	}
}

void cMainGameScene::Render()
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

void cMainGameScene::UIRender()
{
	for each (auto p in m_vecUIObject)
	{
		p->Render(m_pSprite);
	}
}

void cMainGameScene::AddOrderNexusCallback(void * CallBackObj)
{
	cMainGameScene* pThis = (cMainGameScene*)CallBackObj;
	if (!pThis->GetEditOn())
	{
		if (pThis->AddOrderNexus()) pThis->SetEditOn(true);
	}
}

void cMainGameScene::AddOrderInhibitorCallback(void * CallBackObj)
{
	cMainGameScene* pThis = (cMainGameScene*)CallBackObj;
	if (!pThis->GetEditOn())
	{
		if (pThis->AddOrderInhibitor()) pThis->SetEditOn(true);
	}
}

void cMainGameScene::AddOrderTurretCallback(void * CallBackObj)
{
	cMainGameScene* pThis = (cMainGameScene*)CallBackObj;
	if (!pThis->GetEditOn())
	{
		if (pThis->AddOrderTurret()) pThis->SetEditOn(true);
	}
}

void cMainGameScene::AddChaosNexusCallback(void * CallBackObj)
{
	cMainGameScene* pThis = (cMainGameScene*)CallBackObj;
	if (!pThis->GetEditOn())
	{
		if (pThis->AddChaosNexus()) pThis->SetEditOn(true);
	}
}

void cMainGameScene::AddChaosInhibitorCallback(void * CallBackObj)
{
	cMainGameScene* pThis = (cMainGameScene*)CallBackObj;
	if (!pThis->GetEditOn())
	{
		if (pThis->AddChaosInhibitor()) pThis->SetEditOn(true);
	}
}

void cMainGameScene::AddChaosTurretCallback(void * CallBackObj)
{
	cMainGameScene* pThis = (cMainGameScene*)CallBackObj;
	if (!pThis->GetEditOn())
	{
		if (pThis->AddChaosTurret()) pThis->SetEditOn(true);
	}
}

void cMainGameScene::PrevBuildingCallback(void * CallBackObj)
{
	cMainGameScene* pThis = (cMainGameScene*)CallBackObj;

	if (pThis->PrevBuilding()) pThis->SetEditOn(true);
}

void cMainGameScene::NextBuildingCallback(void * CallBackObj)
{
	cMainGameScene* pThis = (cMainGameScene*)CallBackObj;

	if (pThis->NextBuilding()) pThis->SetEditOn(true);
}

void cMainGameScene::SaveBuildingCallback(void * CallBackObj)
{
	cMainGameScene* pThis = (cMainGameScene*)CallBackObj;
	if (!pThis->GetEditOn())
	{
		pThis->SaveBuilding("Buildings.txt");
	}
}

void cMainGameScene::LoadBuildingCallback(void * CallBackObj)
{
	cMainGameScene* pThis = (cMainGameScene*)CallBackObj;
	if (!pThis->GetEditOn())
	{
		pThis->LoadBuilding("Buildings.txt");
	}
}

void cMainGameScene::DeleteBuildingCallback(void * CallBackObj)
{
	cMainGameScene* pThis = (cMainGameScene*)CallBackObj;
	if (pThis->GetEditOn())
	{
		if (pThis->DeleteBuilding()) pThis->SetEditOn(false);
	}
}

void cMainGameScene::EnterBuildingCallback(void * CallBackObj)
{
	cMainGameScene* pThis = (cMainGameScene*)CallBackObj;
	if (pThis->GetEditOn())
	{
		if (pThis->EnterBuilding()) pThis->SetEditOn(false);
	}
}

void cMainGameScene::AlistarWalkCallBack(void * CallBackObj)
{
	cUnit* Alistar = (cUnit*)CallBackObj;
	g_pEffectManager->PlayEffect("effect/walk.eff", Alistar->GetPosition());
}

void cMainGameScene::AlistarSpell1CallBack(void * CallBackObj)
{
	cUnit* Alistar = (cUnit*)CallBackObj;

	Alistar->SetState(STATE_IDLE);
}

void cMainGameScene::AlistarSpell1CallBack2(void * CallBackObj)
{
	cUnit* Alistar = (cUnit*)CallBackObj;

	D3DXVECTOR3 dir = Alistar->GetDirection();

	set<cPhysics*> targets = g_pPhysicsManager->GetTargets(Alistar->GetPosition() + dir * 30, 100);

	cAlphablending* pAlphablending = g_pAlphablending->GetAlphablending("AlistarQ");
	pAlphablending->SetPositition(Alistar->GetPosition() + dir * 30);
	g_pAlphablending->Play("AlistarQ");
	g_pEffectManager->PlayEffect("effect/alistarq.eff", Alistar->GetPosition() + dir * 30);

	for each (auto target in targets)
	{
		target->SetAcceleration(D3DXVECTOR3(0, 0, 0));
		target->SetVelocity(target->GetVelocity() + D3DXVECTOR3(rand() % 100 - 50, 700, rand() % 100 - 50));
		target->SetAngularVelocity(D3DXVECTOR3(rand() % 20 - 10, rand() % 20 - 10, rand() % 20 - 10));
	}
}

void cMainGameScene::AlistarSpell2CallBack2(void * CallBackObj)
{
	cUnit* Alistar = (cUnit*)CallBackObj;

	D3DXVECTOR3 dir = Alistar->GetDirection();

	set<cPhysics*> targets = g_pPhysicsManager->GetTargets(Alistar->GetPosition(), 100);

	for each (auto target in targets)
	{
		target->SetAcceleration(D3DXVECTOR3(0, 0, 0));
		target->SetVelocity(target->GetVelocity() + D3DXVECTOR3(dir.x * 500, 300, dir.z * 500));
		target->SetAngularVelocity(D3DXVECTOR3(rand() % 20 - 10, rand() % 20 - 10, rand() % 20 - 10));
	}
}

void cMainGameScene::AlistarSpell2CallBack(void * CallBackObj)
{
	cUnit* Alistar = (cUnit*)CallBackObj;

	Alistar->SetState(STATE_IDLE);
}

bool cMainGameScene::AddOrderNexus(void)
{
	cBuilding* pOrderNexus = new cBuilding;
	vector<ST_UNITLOADINFO> tempOrderNexus;
	tempOrderNexus.push_back({ STATE_IDLE, "unit/OrderNexus.x", NULL, NULL });
	pOrderNexus->Setup(tempOrderNexus, m_pMap);
	pOrderNexus->SetPosition(D3DXVECTOR3(500, 50, 500));
	pOrderNexus->SetSelect(true);
	g_pCamera->SetTarget(pOrderNexus->GetPosPtr());
	pOrderNexus->SetType(cBuilding::E_ORDERNEXUS);
	m_pCurrentBuilding = pOrderNexus;
	m_vecBuilding.push_back(pOrderNexus);

	return true;
}

bool cMainGameScene::AddOrderInhibitor(void)
{
	cBuilding* pOrderInhibitor = new cBuilding;
	vector<ST_UNITLOADINFO> tempOrderInhibitor;
	tempOrderInhibitor.push_back({ STATE_IDLE, "unit/OrderInhibitor.x", NULL, NULL });
	pOrderInhibitor->Setup(tempOrderInhibitor, m_pMap);
	pOrderInhibitor->SetPosition(D3DXVECTOR3(500, 50, 500));
	pOrderInhibitor->SetSelect(true);
	g_pCamera->SetTarget(pOrderInhibitor->GetPosPtr());
	pOrderInhibitor->SetType(cBuilding::E_ORDERINHIBITOR);
	m_pCurrentBuilding = pOrderInhibitor;
	m_vecBuilding.push_back(pOrderInhibitor);

	return true;
}

bool cMainGameScene::AddOrderTurret(void)
{
	cBuilding* pOrderTurret = new cBuilding;
	vector<ST_UNITLOADINFO> tempOrderTurret;
	tempOrderTurret.push_back({ STATE_IDLE, "unit/OrderTurret.x", NULL, NULL });
	pOrderTurret->Setup(tempOrderTurret, m_pMap);
	pOrderTurret->SetPosition(D3DXVECTOR3(500, 50, 500));
	pOrderTurret->SetSelect(true);
	g_pCamera->SetTarget(pOrderTurret->GetPosPtr());
	pOrderTurret->SetType(cBuilding::E_ORDERTURRET);
	m_pCurrentBuilding = pOrderTurret;
	m_vecBuilding.push_back(pOrderTurret);

	return true;
}

bool cMainGameScene::AddChaosNexus(void)
{
	cBuilding* pChaosNexus = new cBuilding;
	vector<ST_UNITLOADINFO> tempChaosNexus;
	tempChaosNexus.push_back({ STATE_IDLE, "unit/ChaosNexus.x", NULL, NULL });
	pChaosNexus->Setup(tempChaosNexus, m_pMap);
	pChaosNexus->SetPosition(D3DXVECTOR3(500, 50, 500));
	pChaosNexus->SetSelect(true);
	g_pCamera->SetTarget(pChaosNexus->GetPosPtr());
	pChaosNexus->SetType(cBuilding::E_CHAOSNEXUS);
	m_pCurrentBuilding = pChaosNexus;
	m_vecBuilding.push_back(pChaosNexus);

	return true;
}

bool cMainGameScene::AddChaosInhibitor(void)
{
	cBuilding* pChaosInhibitor = new cBuilding;
	vector<ST_UNITLOADINFO> tempChaosInhibitor;
	tempChaosInhibitor.push_back({ STATE_IDLE, "unit/ChaosInhibitor.x", NULL, NULL });
	pChaosInhibitor->Setup(tempChaosInhibitor, m_pMap);
	pChaosInhibitor->SetPosition(D3DXVECTOR3(500, 50, 500));
	pChaosInhibitor->SetSelect(true);
	g_pCamera->SetTarget(pChaosInhibitor->GetPosPtr());
	pChaosInhibitor->SetType(cBuilding::E_CHAOSINHIBITOR);
	m_pCurrentBuilding = pChaosInhibitor;
	m_vecBuilding.push_back(pChaosInhibitor);

	return true;
}

bool cMainGameScene::AddChaosTurret(void)
{
	cBuilding* pChaosTurret = new cBuilding;
	vector<ST_UNITLOADINFO> tempChaosTurret;
	tempChaosTurret.push_back({ STATE_IDLE, "unit/ChaosTurret.x", NULL, NULL });
	pChaosTurret->Setup(tempChaosTurret, m_pMap);
	pChaosTurret->SetPosition(D3DXVECTOR3(500, 50, 500));
	pChaosTurret->SetSelect(true);
	g_pCamera->SetTarget(pChaosTurret->GetPosPtr());
	pChaosTurret->SetType(cBuilding::E_CHAOSTURRET);
	m_pCurrentBuilding = pChaosTurret;
	m_vecBuilding.push_back(pChaosTurret);

	return true;
}

bool cMainGameScene::PrevBuilding()
{
	if (!m_vecBuilding.empty())
	{
		if (m_pCurrentBuilding) m_pCurrentBuilding->SetSelect(false);

		if (m_pPlayer)
		{
			g_pCamera->SetTarget(m_pPlayer->GetPosPtr());
		}

		if (m_nIndexBuilding > 0)
		{
			m_nIndexBuilding--;
		}

		m_pCurrentBuilding = m_vecBuilding[m_nIndexBuilding];
		m_pCurrentBuilding->SetSelect(true);
		g_pCamera->SetTarget(m_pCurrentBuilding->GetPosPtr());

		return true;
	}

	return false;
}

bool cMainGameScene::NextBuilding()
{
	if (!m_vecBuilding.empty())
	{
		if (m_pCurrentBuilding) m_pCurrentBuilding->SetSelect(false);

		if (m_pPlayer)
		{
			g_pCamera->SetTarget(m_pPlayer->GetPosPtr());
		}

		if (m_nIndexBuilding < m_vecBuilding.size() - 1)
		{
			m_nIndexBuilding++;
		}

		m_pCurrentBuilding = m_vecBuilding[m_nIndexBuilding];
		m_pCurrentBuilding->SetSelect(true);
		g_pCamera->SetTarget(m_pCurrentBuilding->GetPosPtr());

		return true;
	}

	return false;
}

bool cMainGameScene::DeleteBuilding()
{
	if (m_pCurrentBuilding)
	{
		for (int i = 0; i < m_vecBuilding.size(); i++)
		{
			if (m_pCurrentBuilding == m_vecBuilding[i])
			{
				SAFE_DELETE(m_pCurrentBuilding);
				m_vecBuilding.erase(m_vecBuilding.begin() + i);
				break;
			}
		}

		EnterBuilding();

		return true;
	}

	return false;
}

bool cMainGameScene::EnterBuilding()
{
	for (int i = 0; i < m_vecBuilding.size(); i++)
	{
		m_vecBuilding[i]->SetSelect(false);
	}

	if (m_pPlayer)
	{
		g_pCamera->SetTarget(m_pPlayer->GetPosPtr());
	}

	m_pCurrentBuilding = NULL;

	return true;
}

bool cMainGameScene::SaveBuilding(char* szFileName)
{
	cBuildingLoader buildingLoader;
	buildingLoader.SaveBuilding(szFileName, m_vecBuilding);

	return true;
}

bool cMainGameScene::LoadBuilding(char* szFileName)
{
	cBuildingLoader buildingLoader;
	buildingLoader.LoadBuilding(szFileName, m_pMap, m_vecBuilding);

	return true;
}
