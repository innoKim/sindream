#include "stdafx.h"
#include "cMainGame.h"
#include "cTestScene.h"

cMainGame::cMainGame() :
	m_pScene(NULL)
{
}

cMainGame::~cMainGame()
{
	SAFE_DELETE(m_pScene);

	g_pKeyManager->Destroy();
	g_pTimeManager->Destroy();
	g_pTextureManager->Destroy();
	g_pDeviceManager->Destroy();
	g_pCamera->Destroy();
}

void cMainGame::Setup()
{
	g_pCamera->Setup();
		
	m_pScene = new cTestScene; //<<-�׽�Ʈ �� ����� ��༮�� �ٲٸ� �˴ϴ�. ��� �翬�� �߰��ϰ�
	m_pScene->Setup();
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

	m_pScene->Render();
	
	/////  ���� ��
	g_pD3DDevice->EndScene();
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}
