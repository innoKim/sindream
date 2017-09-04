#include "stdafx.h"
#include "cMainGame.h"

cMainGame::cMainGame()
{
}

cMainGame::~cMainGame()
{
	g_pKeyManager->Destroy();
	g_pTimeManager->Destroy();
	g_pTextureManager->Destroy();
	g_pDeviceManager->Destroy();
}

void cMainGame::Setup()
{
}

void cMainGame::Update()
{
}

void cMainGame::Render()
{
	g_pD3DDevice->Clear(NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	g_pD3DDevice->BeginScene();
	///// 여기서부터 렌더 시작






	/////  렌더 끝
	g_pD3DDevice->EndScene();
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}
