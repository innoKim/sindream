#include "stdafx.h"
#include "cMainGame.h"
#include "cShader.h"
#include "cCamera.h"

cMainGame::cMainGame()
	: m_pShader(NULL)
	, m_pCamera(NULL)
{
}

cMainGame::~cMainGame()
{
	SAFE_DELETE(m_pShader);
	SAFE_DELETE(m_pCamera);

	g_pKeyManager->Destroy();
	g_pTimeManager->Destroy();
	g_pTextureManager->Destroy();
	g_pDeviceManager->Destroy();
}

void cMainGame::Setup()
{
	cOBJLoader objLoader;
	m_vecVertex = objLoader.Load("LoL/room.obj");

	m_pCamera = new cCamera;
	m_pCamera->Setup(NULL);

	m_pShader = new cShader;
	m_pShader->Setup(m_pCamera->GetEye(), "specularMapping.fx", "sphere.x", "Fieldstone_DM.tga", "Fieldstone_SM.tga");
}

void cMainGame::Update()
{
	if (m_pCamera) m_pCamera->Update();
}

void cMainGame::Render()
{
	g_pD3DDevice->Clear(NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	g_pD3DDevice->BeginScene();
	///// 여기서부터 렌더 시작

	if (m_pShader) m_pShader->Render();

	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	
	D3DXMATRIX	matW;
	D3DXMatrixIdentity(&matW);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matW);

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
	g_pD3DDevice->SetTexture(0, NULL);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vecVertex.size() / 3, &m_vecVertex[0], sizeof(ST_PC_VERTEX));


	

	/////  렌더 끝
	g_pD3DDevice->EndScene();
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void cMainGame::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pCamera)
	{
		m_pCamera->MsgProc(hWnd, message, wParam, lParam);
	}
}
