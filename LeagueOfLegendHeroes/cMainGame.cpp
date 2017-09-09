#include "stdafx.h"
#include "cMainGame.h"
#include "cShader.h"

cMainGame::cMainGame()
	: m_pShader(NULL)
{
}

cMainGame::~cMainGame()
{
	SAFE_DELETE(m_pShader);

	g_pKeyManager->Destroy();
	g_pTimeManager->Destroy();
	g_pTextureManager->Destroy();
	g_pDeviceManager->Destroy();
}

void cMainGame::Setup()
{
	m_pShader = new cShader;
	m_pShader->Setup("lighting.fx", "sphere.x");
}

void cMainGame::Update()
{
}

void cMainGame::Render()
{
	g_pD3DDevice->Clear(NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	g_pD3DDevice->BeginScene();
	///// ���⼭���� ���� ����

	if (m_pShader) m_pShader->Render();

	/////  ���� ��
	g_pD3DDevice->EndScene();
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}
