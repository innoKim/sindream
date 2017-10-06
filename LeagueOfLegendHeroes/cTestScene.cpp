#include "stdafx.h"
#include "cTestScene.h"
#include "cMap.h"
#include "cPlayer.h"
#include "cUnit.h"
#include "cBuilding.h"

cTestScene::cTestScene()
	: m_pPlayer(NULL)
	, m_pMap(NULL)
	, m_bEditOn(false)
{
}


cTestScene::~cTestScene()
{
	SAFE_DELETE(m_pMap);
	SAFE_DELETE(m_pPlayer);
}

void cTestScene::Setup()
{
	m_pMap = new cMap;
	m_pMap->LoadMap("map/", "room.obj");	
	m_pMap->LoadSur("LoL/room_surface.obj");

	m_pPlayer = new cPlayer;
	vector<ST_UNITLOADINFO> temp;
	temp.push_back({ STATE_IDLE, "unit/AlistarIdle.x" });
	temp.push_back({ STATE_RUN, "unit/AlistarRun.x" });
	temp.push_back({ STATE_SPELL1, "unit/AlistarSpell1.x",ST_CallbackInfo(0.0f,AlistarSpell1CallBack,m_pPlayer)});
	temp.push_back({ STATE_SPELL2, "unit/AlistarSpell2.x",{ 0.0f,AlistarSpell2CallBack,m_pPlayer } });
	m_pPlayer->Setup(temp, m_pMap);

	m_pPlayer->SetPosition(D3DXVECTOR3(0.f, 0.f, 0.f));

	//카메라 설정 이렇게 할 수 있습니다.
	g_pCamera->SetTarget(m_pPlayer->GetPosPtr());
	g_pCamera->Zoom(20.f);

	temp.push_back({ STATE_SPELL1, "unit/AlistarSpell1.x",ST_CallbackInfo(0.0f,AlistarSpell1CallBack,m_pPlayer) });
	temp.push_back({ STATE_SPELL2, "unit/AlistarSpell2.x",ST_CallbackInfo(0.0f,AlistarSpell2CallBack,m_pPlayer) });
	m_pPlayer->Setup(temp, m_pMap);
	
	m_pPlayer->SetPosition(D3DXVECTOR3(1000.0f, 0, 1000.0f));

	//카메라 설정 이렇게 할 수 있습니다.
	g_pCamera->SetTarget(m_pPlayer->GetPosPtr());
	g_pCamera->Zoom(5.f);
	
	g_pShaderManager->SetTarget(g_pCamera->GetTarget());

	cBuilding* pOrderNexus = new cBuilding;
	vector<ST_UNITLOADINFO> tempBuilding;
	tempBuilding.push_back({ STATE_IDLE, "unit/ChaosInhibitor.x" ,NULL,NULL });
	pOrderNexus->Setup(tempBuilding, m_pMap);
	pOrderNexus->SetPosition(D3DXVECTOR3(750, 50, 750));
	m_vecBuilding.push_back(pOrderNexus);

	//SetLight();
}

void cTestScene::Update()
{
	if (m_pPlayer)
	{
		m_pPlayer->Update();
		if (m_pMap)	m_pPlayer->SetPosY(m_pMap->GetHeight(m_pPlayer->GetPosition()));
	}

	for (int i = 0; i < m_vecBuilding.size(); i++)
	{
		m_vecBuilding[i]->Update();
	}

	m_pPlayer->Update();
	if (m_pMap)	m_pPlayer->SetPosY(m_pMap->GetHeight(m_pPlayer->GetPosition()));
}

void cTestScene::Render()
{
	m_pPlayer->Render();

	for (int i = 0; i < m_vecBuilding.size(); i++)
	{
		m_vecBuilding[i]->Render();
	}
}

void cTestScene::UIRender()
{
}

void cTestScene::SetLight()
{
	D3DLIGHT9	stLight;
	ZeroMemory(&stLight, sizeof(D3DLIGHT9));
	stLight.Type = D3DLIGHT_DIRECTIONAL;
	stLight.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	stLight.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	stLight.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	stLight.Direction = D3DXVECTOR3(1.f, -1.f, 1.f);

	g_pD3DDevice->SetLight(0, &stLight);
	g_pD3DDevice->LightEnable(0, true);

	g_pD3DDevice->SetRenderState(D3DRS_SPECULARENABLE, true);
}

void cTestScene::AlistarSpell1CallBack(void *CallBackObj)
{
	cUnit* Alistar = (cUnit*)CallBackObj;

	Alistar->SetState(STATE_IDLE);
}

void cTestScene::AlistarSpell2CallBack(void * CallBackObj)
{
	cUnit* Alistar = (cUnit*)CallBackObj;

	Alistar->SetState(STATE_IDLE);
}