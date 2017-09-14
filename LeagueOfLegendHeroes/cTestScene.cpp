#include "stdafx.h"
#include "cTestScene.h"

cTestScene::cTestScene()
	: m_vPos(D3DXVECTOR3(0, 0, 0))
	, m_fAngle(0.f)
	, m_vDir(D3DXVECTOR3(0, 0, 1))
	, m_fSpeed(0.3f)
{
}


cTestScene::~cTestScene()
{
	for each (auto p in m_vecMap)
	{
		SAFE_DELETE(p);
	}
}

void cTestScene::Setup()
{
	//cOBJLoader objLoader;
	//objLoader.Load("map/", "room.obj", m_vecMap);

	cOBJLoader surLoader;
	surLoader.LoadSur("Lol/room_surface.obj", m_vecSur, m_vecGrid);

	//카메라 설정 이렇게 할 수 있습니다.
	g_pCamera->SetTarget(&m_vPos);
	g_pCamera->Zoom(100.0f);
}

void cTestScene::Update()
{	
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

void cTestScene::Render()
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);

	D3DXMatrixIdentity(&m_matW);

	D3DXMatrixScaling(&m_matS, 0.01f, 0.01f, 0.01f);

	m_matW *= m_matS;

	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matW);

	g_pD3DDevice->SetTexture(0, NULL);
	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vecSur.size() / 3, &m_vecSur[0], sizeof(ST_PC_VERTEX));
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST, m_vecGrid.size() / 2, &m_vecGrid[0], sizeof(ST_PC_VERTEX));

	D3DXMatrixRotationX(&m_matR, -D3DX_PI / 2);

	m_matW = m_matS * m_matR;

	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matW);

	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	for each (auto p in m_vecMap)
	{
		p->Render();
	}
}
