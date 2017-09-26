#include "stdafx.h"
#include "cMapEditorScene.h"
#include "cBuilding.h"
#include "cUnit.h"
#include "cPlayer.h"
#include "cPhysics.h"
#include "cMap.h"
#include "cBuildingLoader.h"
#include "cUIObject.h"
#include "cUIButton.h"

cMapEditorScene::cMapEditorScene()
	: m_pPlayer(NULL)
	, m_pMap(NULL)
	, m_pSprite(NULL)
	, m_bEditOn(NULL)
	, m_pCurrentBuilding(NULL)
	, m_nIndexBuilding(0)
{
}


cMapEditorScene::~cMapEditorScene()
{
	SAFE_DELETE(m_pPlayer);
	SAFE_DELETE(m_pMap);
	m_pCurrentBuilding = NULL;

	SAFE_RELEASE(m_pSprite);

	for each (auto p in m_vecBuilding)
	{
		SAFE_DELETE(p);
	}
}

void cMapEditorScene::Setup()
{
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);

	m_pMap = new cMap;
	m_pMap->LoadMap("map/", "room.obj");
	m_pMap->LoadSur("LoL/room_surface.obj");

	m_pPlayer = new cPlayer;
	vector<ST_UNITLOADINFO> temp;
	temp.push_back({ STATE_IDLE, "unit/AlistarIdle.x" });
	temp.push_back({ STATE_RUN, "unit/AlistarRun.x" });
	temp.push_back({ STATE_SPELL1, "unit/AlistarSpell1.x",ST_CallbackInfo(0.0f,AlistarSpell1CallBack,m_pPlayer),ST_CallbackInfo(0.5f,AlistarSpell1CallBack2,m_pPlayer) });
	temp.push_back({ STATE_SPELL2, "unit/AlistarSpell2.x",ST_CallbackInfo(0.0f,AlistarSpell2CallBack,m_pPlayer),ST_CallbackInfo(0.5f,AlistarSpell2CallBack2,m_pPlayer) });
	m_pPlayer->Setup(temp, m_pMap);
	m_pPlayer->SetPosition(D3DXVECTOR3(1000, 0, 1000));
	g_pCamera->SetTarget(m_pPlayer->GetPosPtr());
	g_pShaderManager->SetTarget(g_pCamera->GetTarget());

	cUIObject* buttonSet = new cUIObject;
	m_vecUIObject.push_back(buttonSet);

	for (int i = 0; i < 12; i++)
	{
		char str[128];
		sprintf(str, "button%d", i);
		cUIButton* button = new cUIButton;
		button->SetTag(str);
		buttonSet->AddChild(button);
		button->SetTexture("texture/smallbutton_norm.png", "texture/smallbutton_over.png", "texture/smallbutton_selected.png");
		button->SetPosition(1000 + (i % 2) * 80, ((i / 2) + 1) * 80);
	}

	//물리관련
	m_pPlayer->GetPhysics()->SetIsActivate(false);
}

void cMapEditorScene::Update()
{
	m_pPlayer->Update();
	m_pPlayer->SetPosY(m_pMap->GetHeight(m_pPlayer->GetPosition()));

	if (!m_bEditOn && g_pKeyManager->IsOnceKeyDown('1'))
	{
		m_bEditOn = true;
		cBuilding* pOrderNexus = new cBuilding;
		vector<ST_UNITLOADINFO> tempOrderNexus;
		tempOrderNexus.push_back({ STATE_IDLE, "unit/OrderNexus.x", NULL, NULL });
		pOrderNexus->Setup(tempOrderNexus, m_pMap);
		pOrderNexus->SetPosition(D3DXVECTOR3(500, 50, 500));
		pOrderNexus->SetSelect(true);
		g_pCamera->SetTarget(pOrderNexus->GetPosPtr());
		pOrderNexus->SetType(cBuilding::E_ORDERNEXUS);
		m_vecBuilding.push_back(pOrderNexus);
	}

	if (!m_bEditOn && g_pKeyManager->IsOnceKeyDown('2'))
	{
		m_bEditOn = true;
		cBuilding* pOrderInhibitor = new cBuilding;
		vector<ST_UNITLOADINFO> tempOrderInhibitor;
		tempOrderInhibitor.push_back({ STATE_IDLE, "unit/OrderInhibitor.x", NULL, NULL });
		pOrderInhibitor->Setup(tempOrderInhibitor, m_pMap);
		pOrderInhibitor->SetPosition(D3DXVECTOR3(500, 50, 500));
		pOrderInhibitor->SetSelect(true);
		g_pCamera->SetTarget(pOrderInhibitor->GetPosPtr());
		pOrderInhibitor->SetType(cBuilding::E_ORDERINHIBITOR);
		m_vecBuilding.push_back(pOrderInhibitor);
	}

	if (!m_bEditOn && g_pKeyManager->IsOnceKeyDown('3'))
	{
		m_bEditOn = true;
		cBuilding* pOrderTurret = new cBuilding;
		vector<ST_UNITLOADINFO> tempOrderTurret;
		tempOrderTurret.push_back({ STATE_IDLE, "unit/OrderTurret.x", NULL, NULL });
		pOrderTurret->Setup(tempOrderTurret, m_pMap);
		pOrderTurret->SetPosition(D3DXVECTOR3(500, 50, 500));
		pOrderTurret->SetSelect(true);
		g_pCamera->SetTarget(pOrderTurret->GetPosPtr());
		pOrderTurret->SetType(cBuilding::E_ORDERTURRET);
		m_vecBuilding.push_back(pOrderTurret);
	}

	if (!m_bEditOn && g_pKeyManager->IsOnceKeyDown('4'))
	{
		m_bEditOn = true;
		cBuilding* pChaosNexus = new cBuilding;
		vector<ST_UNITLOADINFO> tempChaosNexus;
		tempChaosNexus.push_back({ STATE_IDLE, "unit/ChaosNexus.x", NULL, NULL });
		pChaosNexus->Setup(tempChaosNexus, m_pMap);
		pChaosNexus->SetPosition(D3DXVECTOR3(500, 50, 500));
		pChaosNexus->SetSelect(true);
		g_pCamera->SetTarget(pChaosNexus->GetPosPtr());
		pChaosNexus->SetType(cBuilding::E_CHAOSNEXUS);
		m_vecBuilding.push_back(pChaosNexus);
	}

	if (!m_bEditOn && g_pKeyManager->IsOnceKeyDown('5'))
	{
		m_bEditOn = true;
		cBuilding* pChaosInhibitor = new cBuilding;
		vector<ST_UNITLOADINFO> tempChaosInhibitor;
		tempChaosInhibitor.push_back({ STATE_IDLE, "unit/ChaosInhibitor.x", NULL, NULL });
		pChaosInhibitor->Setup(tempChaosInhibitor, m_pMap);
		pChaosInhibitor->SetPosition(D3DXVECTOR3(500, 50, 500));
		pChaosInhibitor->SetSelect(true);
		g_pCamera->SetTarget(pChaosInhibitor->GetPosPtr());
		pChaosInhibitor->SetType(cBuilding::E_CHAOSINHIBITOR);
		m_vecBuilding.push_back(pChaosInhibitor);
	}

	if (!m_bEditOn && g_pKeyManager->IsOnceKeyDown('6'))
	{
		m_bEditOn = true;
		cBuilding* pChaosTurret = new cBuilding;
		vector<ST_UNITLOADINFO> tempChaosTurret;
		tempChaosTurret.push_back({ STATE_IDLE, "unit/ChaosTurret.x", NULL, NULL });
		pChaosTurret->Setup(tempChaosTurret, m_pMap);
		pChaosTurret->SetPosition(D3DXVECTOR3(500, 50, 500));
		pChaosTurret->SetSelect(true);
		g_pCamera->SetTarget(pChaosTurret->GetPosPtr());
		pChaosTurret->SetType(cBuilding::E_CHAOSTURRET);
		m_vecBuilding.push_back(pChaosTurret);
	}

	if (g_pKeyManager->IsStayKeyDown(VK_CONTROL))
	{
		if (g_pKeyManager->IsOnceKeyDown('S'))
		{
			cBuildingLoader buildingLoader;
			buildingLoader.SaveBuilding(m_vecBuilding);
		}

		if (g_pKeyManager->IsOnceKeyDown('L'))
		{
			cBuildingLoader buildingLoader;
			buildingLoader.LoadBuilding(m_pMap, m_vecBuilding);
		}
	}

	if (g_pKeyManager->IsStayKeyDown(VK_RETURN))
	{
		m_bEditOn = false;

		for (int i = 0; i < m_vecBuilding.size(); i++)
		{
			m_vecBuilding[i]->SetSelect(false);
		}

		if (m_pPlayer)
		{
			g_pCamera->SetTarget(m_pPlayer->GetPosPtr());
		}
	}

	if (g_pKeyManager->IsOnceKeyDown(VK_LEFT))
	{
		m_vecBuilding[m_nIndexBuilding]->SetSelect(false);

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
	}

	if (g_pKeyManager->IsOnceKeyDown(VK_RIGHT))
	{
		m_vecBuilding[m_nIndexBuilding]->SetSelect(false);

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
	}

	for each (auto p in m_vecUIObject)
	{
		p->Update();
	}

	for each(auto p in m_vecBuilding)
	{
		p->Update();
	}
}

void cMapEditorScene::Render()
{
	m_pPlayer->Render();

	for each(auto p in m_vecBuilding)
	{
		p->Render();
	}
}

void cMapEditorScene::UIRender()
{
	for each (auto p in m_vecUIObject)
	{
		p->Render(m_pSprite);
	}
}

void cMapEditorScene::AlistarSpell1CallBack(void * CallBackObj)
{
	cUnit* Alistar = (cUnit*)CallBackObj;

	Alistar->SetState(STATE_IDLE);
}

void cMapEditorScene::AlistarSpell1CallBack2(void * CallBackObj)
{
	cUnit* Alistar = (cUnit*)CallBackObj;

	set<cPhysics*> targets = g_pPhysicsManager->GetTargets(Alistar->GetPosition(), 100);

	D3DXVECTOR3 dir = Alistar->GetDirection();

	for each (auto target in targets)
	{
		target->SetAcceleration(D3DXVECTOR3(0, 0, 0));
		target->SetVelocity(target->GetVelocity() + D3DXVECTOR3(dir.x * 500, 100, dir.z * 500));
		target->SetAngularVelocity(D3DXVECTOR3(rand() % 20 - 10, rand() % 20 - 10, rand() % 20 - 10));
	}
}

void cMapEditorScene::AlistarSpell2CallBack2(void * CallBackObj)
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

void cMapEditorScene::AlistarSpell2CallBack(void * CallBackObj)
{
	cUnit* Alistar = (cUnit*)CallBackObj;

	Alistar->SetState(STATE_IDLE);
}

D3DXVECTOR3 cMapEditorScene::playerPos()
{
	return m_pPlayer->GetPosition();
}
