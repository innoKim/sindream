#include "stdafx.h"
#include "cTestScene.h"

cTestScene::cTestScene()
	: m_pPlayer(NULL)
	, m_pMap(NULL)
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
	m_pPlayer->Setup(temp);

	m_pPlayer->SetPosition(D3DXVECTOR3(0.f, 0.f, 0.f));

	//카메라 설정 이렇게 할 수 있습니다.
	g_pCamera->SetTarget(m_pPlayer->GetPosPtr());
	g_pCamera->Zoom(5.f);
	
	g_pShaderManager->SetTarget(g_pCamera->GetTarget());

	//SetLight();
}

void cTestScene::Update()
{
	m_pPlayer->Update();
	m_pPlayer->SetPosY(m_pMap->GetGroundHeight(m_pPlayer->GetPosition()));
}

void cTestScene::Render()
{
	m_pPlayer->Render();
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