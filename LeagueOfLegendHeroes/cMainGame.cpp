#include "stdafx.h"
#include "cMainGame.h"
#include "cShader.h"
#include "cCamera.h"

cMainGame::cMainGame()
	: m_pShader(NULL)
	, m_pCamera(NULL)
	, m_vPos(D3DXVECTOR3(0, 0, 0))
	, m_fAngle(0.f)
	, m_vDir(D3DXVECTOR3(0, 0, 1))
	, m_fSpeed(0.1f)
{
}

cMainGame::~cMainGame()
{
	SAFE_DELETE(m_pShader);
	SAFE_DELETE(m_pCamera);

	for each (auto p in m_vecGroup)
	{
		SAFE_DELETE(p);
	}

	g_pKeyManager->Destroy();
	g_pTimeManager->Destroy();
	g_pTextureManager->Destroy();
	g_pDeviceManager->Destroy();
}

void cMainGame::Setup()
{
	cOBJLoader objLoader;
	objLoader.Load("map/", "room.obj", m_vecGroup);

	m_pCamera = new cCamera;
	//m_pCamera->Setup(NULL);
	// 카메라 테스트용
	
	m_pCamera->Setup(&m_vPos);

	// 끝

	m_pShader = new cShader;
	m_pShader->Setup(m_pCamera->GetEye(), "shader/specularMapping.fx", "shader/sphere.x", "shader/Fieldstone_DM.tga", "shader/Fieldstone_SM.tga");
}

void cMainGame::Update()
{
	if (m_pCamera) m_pCamera->Update();

	// 카메라 테스트용
	if (g_pKeyManager->IsStayKeyDown('W'))
	{
		m_vPos -= m_vDir * m_fSpeed;
	}

	if (g_pKeyManager->IsStayKeyDown('S'))
	{
		m_vPos += m_vDir * m_fSpeed;
	}
	if (g_pKeyManager->IsStayKeyDown('A'))
	{
		m_fAngle -= 0.05f;
	}
	if (g_pKeyManager->IsStayKeyDown('D'))
	{
		m_fAngle += 0.05f;
	}
	
	D3DXMatrixTranslation(&m_matT, m_vPos.x, m_vPos.y, m_vPos.z);

	m_vDir = D3DXVECTOR3(0, 0, -1);
	D3DXMatrixRotationY(&m_matR, m_fAngle);
	D3DXVec3TransformNormal(&m_vDir, &m_vDir, &m_matR);

	m_matW = m_matR * m_matT;

	// 끝
}

void cMainGame::Render()
{
	g_pD3DDevice->Clear(NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	g_pD3DDevice->BeginScene();
	///// 여기서부터 렌더 시작

	if (m_pShader) m_pShader->Render();

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);

	D3DXMatrixIdentity(&m_matW);

	D3DXMatrixScaling(&m_matS, 0.01f, 0.01f, 0.01f);

	D3DXMatrixRotationX(&m_matR, -D3DX_PI / 2);

	m_matW = m_matS * m_matR;

	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matW);

	for each (auto p in m_vecGroup)
	{
		p->Render();
	}
	
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
