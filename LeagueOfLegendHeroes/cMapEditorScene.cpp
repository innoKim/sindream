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
#include "cUIText.h"
#include "cAlphablending.h"
#include "cAStar.h"
#include "cAStarGrid.h"
#include "cEnemy.h"

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

	for each(auto p in m_vecEnemy)
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

	string buttonTag[12];
	buttonTag[0] = "블루 넥서스";
	buttonTag[1] = "레드 넥서스";
	buttonTag[2] = "블루 억제기";
	buttonTag[3] = "레드 억제기";
	buttonTag[4] = "블루 타워";
	buttonTag[5] = "레드 타워";
	buttonTag[6] = "이전 건물";
	buttonTag[7] = "다음 건물";
	buttonTag[8] = "건물 삭제";
	buttonTag[9] = "건물 세우기";
	buttonTag[10] = "저장";
	buttonTag[11] = "불러오기";

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
		button->SetPosition(750 + (i % 6) * 80, (i / 6) * 80 + 50);

		cUIText* text = new cUIText;
		text->SetTag(buttonTag[i]);
		text->SetText(buttonTag[i]);
		text->SetFontType(cFontManager::E_SMALL);
		text->SetWidth(50);
		text->SetHeight(50);
		text->SetDrawTextFormat(DT_CENTER | DT_VCENTER | DT_WORDBREAK);
		text->SetTag("text");
		button->AddChild(text);
	}

	cUIButton* pButton = (cUIButton*)m_vecUIObject[0]->GetChild("button0");
	pButton->SetCallback(AddOrderNexusCallback);
	pButton->SetCallbackObject(this);

	pButton = (cUIButton*)m_vecUIObject[0]->GetChild("button1");
	pButton->SetCallback(AddChaosNexusCallback);
	pButton->SetCallbackObject(this);

	pButton = (cUIButton*)m_vecUIObject[0]->GetChild("button2");
	pButton->SetCallback(AddOrderInhibitorCallback);
	pButton->SetCallbackObject(this);

	pButton = (cUIButton*)m_vecUIObject[0]->GetChild("button3");
	pButton->SetCallback(AddChaosInhibitorCallback);
	pButton->SetCallbackObject(this);

	pButton = (cUIButton*)m_vecUIObject[0]->GetChild("button4");
	pButton->SetCallback(AddOrderTurretCallback);
	pButton->SetCallbackObject(this);

	pButton = (cUIButton*)m_vecUIObject[0]->GetChild("button5");
	pButton->SetCallback(AddChaosTurretCallback);
	pButton->SetCallbackObject(this);

	pButton = (cUIButton*)m_vecUIObject[0]->GetChild("button6");
	pButton->SetCallback(PrevBuildingCallback);
	pButton->SetCallbackObject(this);

	pButton = (cUIButton*)m_vecUIObject[0]->GetChild("button7");
	pButton->SetCallback(NextBuildingCallback);
	pButton->SetCallbackObject(this);

	pButton = (cUIButton*)m_vecUIObject[0]->GetChild("button8");
	pButton->SetCallback(DeleteBuildingCallback);
	pButton->SetCallbackObject(this);

	pButton = (cUIButton*)m_vecUIObject[0]->GetChild("button9");
	pButton->SetCallback(EnterBuildingCallback);
	pButton->SetCallbackObject(this);

	pButton = (cUIButton*)m_vecUIObject[0]->GetChild("button10");
	pButton->SetCallback(SaveBuildingCallback);
	pButton->SetCallbackObject(this);

	pButton = (cUIButton*)m_vecUIObject[0]->GetChild("button11");
	pButton->SetCallback(LoadBuildingCallback);
	pButton->SetCallbackObject(this);

	g_pAlphablending->AddAlphablending("AlistarQ", "AlistarQgroundcrack.dds", m_pPlayer->GetPosition(), D3DXVECTOR3(0, 1, 0), 1.0f, 150, true, false);

	for (int i = 0; i < 3; i++)
	{
		cUnit* enemy = new cEnemy;
		vector<ST_UNITLOADINFO> temp;
		temp.push_back({ STATE_IDLE, "unit/PoroIdle.x" ,NULL,NULL });
		enemy->Setup(temp, m_pMap);
		enemy->SetPosition(D3DXVECTOR3(1000 + i * 100, 100, 1000 + i * 10));
		m_vecEnemy.push_back(enemy);
	}

	//물리관련
	m_pPlayer->GetPhysics()->SetIsActivate(false);
}

void cMapEditorScene::Update()
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

void cMapEditorScene::AddOrderNexusCallback(void * CallBackObj)
{
	cMapEditorScene* pThis = (cMapEditorScene*)CallBackObj;
	if (!pThis->GetEditOn())
	{
		if (pThis->AddOrderNexus()) pThis->SetEditOn(true);
	}
}

void cMapEditorScene::AddOrderInhibitorCallback(void * CallBackObj)
{
	cMapEditorScene* pThis = (cMapEditorScene*)CallBackObj;
	if (!pThis->GetEditOn())
	{
		if (pThis->AddOrderInhibitor()) pThis->SetEditOn(true);
	}
}

void cMapEditorScene::AddOrderTurretCallback(void * CallBackObj)
{
	cMapEditorScene* pThis = (cMapEditorScene*)CallBackObj;
	if (!pThis->GetEditOn())
	{
		if (pThis->AddOrderTurret()) pThis->SetEditOn(true);
	}
}

void cMapEditorScene::AddChaosNexusCallback(void * CallBackObj)
{
	cMapEditorScene* pThis = (cMapEditorScene*)CallBackObj;
	if (!pThis->GetEditOn())
	{
		if (pThis->AddChaosNexus()) pThis->SetEditOn(true);
	}
}

void cMapEditorScene::AddChaosInhibitorCallback(void * CallBackObj)
{
	cMapEditorScene* pThis = (cMapEditorScene*)CallBackObj;
	if (!pThis->GetEditOn())
	{
		if (pThis->AddChaosInhibitor()) pThis->SetEditOn(true);
	}
}

void cMapEditorScene::AddChaosTurretCallback(void * CallBackObj)
{
	cMapEditorScene* pThis = (cMapEditorScene*)CallBackObj;
	if (!pThis->GetEditOn())
	{
		if (pThis->AddChaosTurret()) pThis->SetEditOn(true);
	}
}

void cMapEditorScene::PrevBuildingCallback(void * CallBackObj)
{
	cMapEditorScene* pThis = (cMapEditorScene*)CallBackObj;
	
	if (pThis->PrevBuilding()) pThis->SetEditOn(true);
}

void cMapEditorScene::NextBuildingCallback(void * CallBackObj)
{
	cMapEditorScene* pThis = (cMapEditorScene*)CallBackObj;
	
	if (pThis->NextBuilding()) pThis->SetEditOn(true);
}

void cMapEditorScene::SaveBuildingCallback(void * CallBackObj)
{
	cMapEditorScene* pThis = (cMapEditorScene*)CallBackObj;
	if (!pThis->GetEditOn())
	{
		pThis->SaveBuilding("BuildingTest.txt");
	}
}

void cMapEditorScene::LoadBuildingCallback(void * CallBackObj)
{
	cMapEditorScene* pThis = (cMapEditorScene*)CallBackObj;
	if (!pThis->GetEditOn())
	{
		pThis->LoadBuilding("BuildingTest.txt");
	}
}

void cMapEditorScene::DeleteBuildingCallback(void * CallBackObj)
{
	cMapEditorScene* pThis = (cMapEditorScene*)CallBackObj;
	if (pThis->GetEditOn())
	{
		if (pThis->DeleteBuilding()) pThis->SetEditOn(false);
	}
}

void cMapEditorScene::EnterBuildingCallback(void * CallBackObj)
{
	cMapEditorScene* pThis = (cMapEditorScene*)CallBackObj;
	if (pThis->GetEditOn())
	{
		if (pThis->EnterBuilding()) pThis->SetEditOn(false);
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

	D3DXVECTOR3 dir = Alistar->GetDirection();

	set<cPhysics*> targets = g_pPhysicsManager->GetTargets(Alistar->GetPosition() + dir * 30, 100);

	cAlphablending* pAlphablending = g_pAlphablending->GetAlphablending("AlistarQ");
	pAlphablending->SetPositition(Alistar->GetPosition() + dir * 30);
	g_pAlphablending->Play("AlistarQ");

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

bool cMapEditorScene::AddOrderNexus(void)
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

bool cMapEditorScene::AddOrderInhibitor(void)
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

bool cMapEditorScene::AddOrderTurret(void)
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

bool cMapEditorScene::AddChaosNexus(void)
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

bool cMapEditorScene::AddChaosInhibitor(void)
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

bool cMapEditorScene::AddChaosTurret(void)
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

bool cMapEditorScene::PrevBuilding()
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

bool cMapEditorScene::NextBuilding()
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

bool cMapEditorScene::DeleteBuilding()
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

bool cMapEditorScene::EnterBuilding()
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

bool cMapEditorScene::SaveBuilding(char* szFileName)
{
	cBuildingLoader buildingLoader;
	buildingLoader.SaveBuilding(szFileName, m_vecBuilding);

	return true;
}

bool cMapEditorScene::LoadBuilding(char* szFileName)
{
	cBuildingLoader buildingLoader;
	buildingLoader.LoadBuilding(szFileName, m_pMap, m_vecBuilding);

	return true;
}
