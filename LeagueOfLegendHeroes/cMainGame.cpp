#include "stdafx.h"
#include "cMainGame.h"
#include "cTestScene.h"
#include "cPhysicsScene.h"
#include "cGridPlane.h"
#include "cParticleScene.h"
#include "cMapEditorScene.h"

cMainGame::cMainGame() :
	m_pScene(NULL),
	m_pGrid(NULL)
{
}

cMainGame::~cMainGame()
{
	SAFE_DELETE(m_pScene);
	SAFE_DELETE(m_pGrid);

	g_pCamera->Destroy();
	g_pCollisionCalculator->Destroy();
	g_pPhysicsManager->Destroy();	
	g_pTextureManager->Destroy();
	g_pFontManager->Destroy();
	g_pDebug->Destroy();
	g_pKeyManager->Destroy();
	g_pTimeManager->Destroy();
	g_pShaderManager->Destroy();
	g_pDeviceManager->Destroy();
	g_pEffectManager->Destroy();
}

void cMainGame::Setup()
{
//	m_pScene = new cTestScene; //<<-테스트 씬 만들면 요녀석만 바꾸면 됩니다. 헤더 당연히 추가하고
//	m_pScene = new cPhysicsScene; // 인호-물리 테스트씬
	m_pScene = new cParticleScene;
//	m_pScene = new cMapEditorScene;

	m_pScene->Setup();

	//m_pGrid = new cGridPlane;
	//m_pGrid->Setup(100, 50);

	g_pShaderManager->SetupShadow();
	g_pEffectManager->LoadEffects();
}

void cMainGame::Update()
{
	m_pScene->Update();

	g_pCamera->Update();
	g_pEffectManager->Update();
}

void cMainGame::Render()
{
	g_pD3DDevice->Clear(NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	g_pD3DDevice->BeginScene();
	///// 여기서부터 렌더 시작

	g_pShaderManager->BeginRender();

	m_pScene->Render();

	g_pShaderManager->Render();
	
	
	g_pEffectManager->Render();

	if (m_pGrid) m_pGrid->Render();
	m_pScene->UIRender();

	/////// 디버깅 폰트 내용

	char str[256];
	sprintf(str, "%.2f FPS", g_pTimeManager->GetFrameRate());
	g_pDebug->Print(str);
	
	sprintf(str, "%.2f, %.2f, %.2f", m_pScene->playerPos().x, m_pScene->playerPos().y, m_pScene->playerPos().z);
	g_pDebug->Print(str,0,100);
	
	/////  렌더 끝
	g_pD3DDevice->EndScene();
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}
