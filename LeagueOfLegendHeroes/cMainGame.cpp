#include "stdafx.h"
#include "cMainGame.h"
#include "cTestScene.h"
#include "cPhysicsScene.h"
#include "cGridPlane.h"

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
	g_pKeyManager->Destroy();
	g_pTimeManager->Destroy();
	g_pTextureManager->Destroy();
	g_pDeviceManager->Destroy();
	g_pDebug->Destroy();
	g_pCollisionCalculator->Destroy();
}

void cMainGame::Setup()
{
	m_pScene = new cTestScene; //<<-�׽�Ʈ �� ����� ��༮�� �ٲٸ� �˴ϴ�. ��� �翬�� �߰��ϰ�
	//m_pScene = new cPhysicsScene; // ��ȣ-���� �׽�Ʈ��
	m_pScene->Setup();

	//m_pGrid = new cGridPlane;
	//m_pGrid->Setup(100,50);


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
	///// ���⼭���� ���� ����

	g_pShaderManager->BeginRender();

	m_pScene->Render();

	g_pShaderManager->Render();
	
	if (m_pGrid) m_pGrid->Render();


	char str[256];
	sprintf(str, "%.2f FPS", g_pTimeManager->GetFrameRate());
	g_pDebug->Print(str);
	
	/////  ���� ��
	g_pD3DDevice->EndScene();
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}
