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
//	m_pScene = new cTestScene;
//	m_pScene = new cParticleScene;
//	m_pScene = new cPhysicsScene;
	m_pScene = new cMapEditorScene;

	m_pScene->Setup();
	
	g_pShaderManager->SetupShadow();
}

void cMainGame::Update()
{
	m_pScene->Update();

	g_pCamera->Update();
	g_pEffectManager->Update();
	g_pAlphablending->Update();
}

void cMainGame::Render()
{
	g_pD3DDevice->Clear(NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	g_pD3DDevice->BeginScene();
	///// ���⼭���� ���� ����

	g_pShaderManager->BeginRender();

	m_pScene->Render();

	g_pShaderManager->Render();
	
	g_pEffectManager->Render();

	g_pAlphablending->UIRender();

	if (m_pGrid) m_pGrid->Render();
	
	m_pScene->UIRender();

	/////// ����� ��Ʈ ����

	char str[256];
	sprintf(str, "%.2f FPS", g_pTimeManager->GetFrameRate());
	g_pDebug->Print(str);
	
	/////  ���� ��
	g_pD3DDevice->EndScene();
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}
