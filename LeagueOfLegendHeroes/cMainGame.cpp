#include "stdafx.h"
#include "cMainGame.h"
#include "cTestScene.h"
#include "cPhysicsScene.h"
cMainGame::cMainGame() :
	m_pScene(NULL)
{
}

cMainGame::~cMainGame()
{
	SAFE_DELETE(m_pScene);
	
	g_pCamera->Destroy();
	g_pKeyManager->Destroy();
	g_pTimeManager->Destroy();
	g_pTextureManager->Destroy();
	g_pDeviceManager->Destroy();
	g_pDebug->Destroy();
	g_pCollisionCalculator->Destroy();
}

void cMainGame::Setup()
{
	m_pScene = new cTestScene; //<<-테스트 씬 만들면 요녀석만 바꾸면 됩니다. 헤더 당연히 추가하고
	//m_pScene = new cPhysicsScene; // 인호-물리 테스트씬
	m_pScene->Setup();

	g_pShaderManager->SetupShadow();
}

void cMainGame::Update()
{
	g_pCamera->Update();
	m_pScene->Update();
}

void cMainGame::Render()
{
	g_pD3DDevice->Clear(NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	g_pD3DDevice->BeginScene();
	///// 여기서부터 렌더 시작

	g_pShaderManager->BeginRender();

	m_pScene->Render();

	g_pShaderManager->Render();
	
	char str[256];
	sprintf(str, "%.2f FPS", g_pTimeManager->GetFrameRate());
	g_pDebug->Print(str);
	
	/////  렌더 끝
	g_pD3DDevice->EndScene();
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}
