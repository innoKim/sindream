#include "stdafx.h"
#include "cPhysicsScene.h"
#include "cBuilding.h"
#include "cUnit.h"
#include "cPlayer.h"
#include "cEnemy.h"
#include "cPhysics.h"
#include "cRigidbody.h"
#include "cMap.h"

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
	m_bEditOn(false)
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
		tempChaosTurret.push_back({ STATE_IDLE, "unit/OrderTurret.x", NULL, NULL });
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
			SaveBuilding();
		}

		if (g_pKeyManager->IsOnceKeyDown('L'))
		{
			LoadBuilding();
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

	g_pPhysicsManager->RigidbodyUpdate();
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

void cPhysicsScene::SaveBuilding()
{
	HANDLE file;
	DWORD write;

	string strData;

	strData += to_string(m_vecBuilding.size()) + '\n';

	for (int i = 0; i < m_vecBuilding.size(); i++)
	{
		switch (m_vecBuilding[i]->GetType())
		{
		case cBuilding::E_ORDERNEXUS:
			strData += "ORDERNEXUS ";
			break;
		case cBuilding::E_ORDERINHIBITOR:
			strData += "ORDERINHIBITOR ";
			break;
		case cBuilding::E_ORDERTURRET:
			strData += "ORDERTURRET ";
			break;
		case cBuilding::E_CHAOSNEXUS:
			strData += "CHAOSNEXUS ";
			break;
		case cBuilding::E_CHAOSINHIBITOR:
			strData += "CHAOSINHIBITOR ";
			break;
		case cBuilding::E_CHAOSTURRET:
			strData += "CHAOSTURRET ";
			break;
		}

		strData += to_string(m_vecBuilding[i]->GetPosition().x) + " ";
		strData += to_string(m_vecBuilding[i]->GetPosition().y) + " ";
		strData += to_string(m_vecBuilding[i]->GetPosition().z) + " ";

		strData += to_string(m_vecBuilding[i]->GetDirection().x) + " ";
		strData += to_string(m_vecBuilding[i]->GetDirection().y) + " ";
		strData += to_string(m_vecBuilding[i]->GetDirection().z) + " ";

		strData += '\n';
	}

	file = CreateFile("unit/buildings.txt", GENERIC_WRITE, 0,
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(file, strData.c_str(), strData.length(), &write, NULL);
	CloseHandle(file);
}

void cPhysicsScene::LoadBuilding()
{
	ifstream inFile("unit/buildings.txt", ios::in);

	vector<string> vecStrMap;

	while (!inFile.eof())
	{
		string str;
		inFile >> str;
		vecStrMap.push_back(str);
	}

	for (int i = 1; i < vecStrMap.size(); i += 7)
	{
		if (vecStrMap[i] == "ORDERNEXUS")
		{
			cBuilding* pOrderNexus = new cBuilding;
			vector<ST_UNITLOADINFO> tempOrderNexus;
			tempOrderNexus.push_back({ STATE_IDLE, "unit/OrderNexus.x", NULL, NULL });
			pOrderNexus->Setup(tempOrderNexus, m_pMap);
			pOrderNexus->SetPosition(D3DXVECTOR3(atof(vecStrMap[i + 1].c_str()), atof(vecStrMap[i + 2].c_str()), atof(vecStrMap[i + 3].c_str())));
			pOrderNexus->SetDirection(D3DXVECTOR3(atof(vecStrMap[i + 4].c_str()), atof(vecStrMap[i + 5].c_str()), atof(vecStrMap[i + 6].c_str())));
			pOrderNexus->SetType(cBuilding::E_ORDERNEXUS);
			m_vecBuilding.push_back(pOrderNexus);
		}
		else if (vecStrMap[i] == "ORDERINHIBITOR")
		{
			cBuilding* pOrderInhibitor = new cBuilding;
			vector<ST_UNITLOADINFO> tempOrderInhibitor;
			tempOrderInhibitor.push_back({ STATE_IDLE, "unit/OrderInhibitor.x", NULL, NULL });
			pOrderInhibitor->Setup(tempOrderInhibitor, m_pMap);
			pOrderInhibitor->SetPosition(D3DXVECTOR3(atof(vecStrMap[i + 1].c_str()), atof(vecStrMap[i + 2].c_str()), atof(vecStrMap[i + 3].c_str())));
			pOrderInhibitor->SetDirection(D3DXVECTOR3(atof(vecStrMap[i + 4].c_str()), atof(vecStrMap[i + 5].c_str()), atof(vecStrMap[i + 6].c_str())));
			pOrderInhibitor->SetType(cBuilding::E_ORDERINHIBITOR);
			m_vecBuilding.push_back(pOrderInhibitor);
		}
		else if (vecStrMap[i] == "ORDERTURRET")
		{
			cBuilding* pOrderTurret = new cBuilding;
			vector<ST_UNITLOADINFO> tempOrderTurret;
			tempOrderTurret.push_back({ STATE_IDLE, "unit/OrderTurret.x", NULL, NULL });
			pOrderTurret->Setup(tempOrderTurret, m_pMap);
			pOrderTurret->SetPosition(D3DXVECTOR3(atof(vecStrMap[i + 1].c_str()), atof(vecStrMap[i + 2].c_str()), atof(vecStrMap[i + 3].c_str())));
			pOrderTurret->SetDirection(D3DXVECTOR3(atof(vecStrMap[i + 4].c_str()), atof(vecStrMap[i + 5].c_str()), atof(vecStrMap[i + 6].c_str())));
			pOrderTurret->SetType(cBuilding::E_ORDERTURRET);
			m_vecBuilding.push_back(pOrderTurret);
		}
		else if (vecStrMap[i] == "CHAOSNEXUS")
		{
			cBuilding* pChaosNexus = new cBuilding;
			vector<ST_UNITLOADINFO> tempChaosNexus;
			tempChaosNexus.push_back({ STATE_IDLE, "unit/ChaosNexus.x", NULL, NULL });
			pChaosNexus->Setup(tempChaosNexus, m_pMap);
			pChaosNexus->SetPosition(D3DXVECTOR3(atof(vecStrMap[i + 1].c_str()), atof(vecStrMap[i + 2].c_str()), atof(vecStrMap[i + 3].c_str())));
			pChaosNexus->SetDirection(D3DXVECTOR3(atof(vecStrMap[i + 4].c_str()), atof(vecStrMap[i + 5].c_str()), atof(vecStrMap[i + 6].c_str())));
			pChaosNexus->SetType(cBuilding::E_CHAOSNEXUS);
			m_vecBuilding.push_back(pChaosNexus);
		}
		else if (vecStrMap[i] == "CHAOSINHIBITOR")
		{
			cBuilding* pChaosInhibitor = new cBuilding;
			vector<ST_UNITLOADINFO> tempChaosInhibitor;
			tempChaosInhibitor.push_back({ STATE_IDLE, "unit/ChaosInhibitor.x", NULL, NULL });
			pChaosInhibitor->Setup(tempChaosInhibitor, m_pMap);
			pChaosInhibitor->SetPosition(D3DXVECTOR3(atof(vecStrMap[i + 1].c_str()), atof(vecStrMap[i + 2].c_str()), atof(vecStrMap[i + 3].c_str())));
			pChaosInhibitor->SetDirection(D3DXVECTOR3(atof(vecStrMap[i + 4].c_str()), atof(vecStrMap[i + 5].c_str()), atof(vecStrMap[i + 6].c_str())));
			pChaosInhibitor->SetType(cBuilding::E_CHAOSINHIBITOR);
			m_vecBuilding.push_back(pChaosInhibitor);
		}
		else if (vecStrMap[i] == "CHAOSTURRET")
		{
			cBuilding* pChaosTurret = new cBuilding;
			vector<ST_UNITLOADINFO> tempChaosTurret;
			tempChaosTurret.push_back({ STATE_IDLE, "unit/OrderTurret.x", NULL, NULL });
			pChaosTurret->Setup(tempChaosTurret, m_pMap);
			pChaosTurret->SetPosition(D3DXVECTOR3(atof(vecStrMap[i + 1].c_str()), atof(vecStrMap[i + 2].c_str()), atof(vecStrMap[i + 3].c_str())));
			pChaosTurret->SetDirection(D3DXVECTOR3(atof(vecStrMap[i + 4].c_str()), atof(vecStrMap[i + 5].c_str()), atof(vecStrMap[i + 6].c_str())));
			pChaosTurret->SetType(cBuilding::E_CHAOSTURRET);
			m_vecBuilding.push_back(pChaosTurret);
		}
	}

	vecStrMap;

	inFile.close();
}

D3DXVECTOR3 cPhysicsScene::playerPos()
{
	return m_pPlayer->GetPosition();
}
