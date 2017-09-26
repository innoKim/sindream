#include "stdafx.h"
#include "cBuilding.h"
#include "cPhysics.h"

cBuilding::cBuilding()
	: m_bSelect(false)
	, m_fSpeed(1.0f)
{
}


cBuilding::~cBuilding()
{
	Destroy();
}

void cBuilding::Setup(vector<ST_UNITLOADINFO> statesVector, cMap * mapPtr)
{
	cUnit::Setup(statesVector, mapPtr);
	m_pPhysics->SetIsActivate(false);
}

void cBuilding::Update()
{
	cUnit::Update();
	KeyControl();
}

void cBuilding::KeyControl()
{
	if (m_bSelect && g_pKeyManager->IsStayKeyDown('W'))
	{
		m_vPos.z += m_fSpeed;
	}
	
	if (m_bSelect && g_pKeyManager->IsStayKeyDown('S'))
	{
		m_vPos.z -= m_fSpeed;
	}

	if (m_bSelect && g_pKeyManager->IsStayKeyDown('A'))
	{
		m_vPos.x -= m_fSpeed;
	}

	if (m_bSelect && g_pKeyManager->IsStayKeyDown('D'))
	{
		m_vPos.x += m_fSpeed;
	}

	if (m_bSelect && g_pKeyManager->IsStayKeyDown('R'))
	{
		m_vPos.y += m_fSpeed;
	}

	if (m_bSelect && g_pKeyManager->IsStayKeyDown('F'))
	{
		m_vPos.y -= m_fSpeed;
	}

	if (m_bSelect && g_pKeyManager->IsStayKeyDown('Q'))
	{
		D3DXMATRIXA16 matR;
		D3DXMatrixRotationY(&matR, -0.01f);
		D3DXVec3TransformNormal(&m_vDir, &m_vDir, &matR);
	}

	if (m_bSelect && g_pKeyManager->IsStayKeyDown('E'))
	{
		D3DXMATRIXA16 matR;
		D3DXMatrixRotationY(&matR,  0.01f);
		D3DXVec3TransformNormal(&m_vDir, &m_vDir, &matR);
	}

	if (m_bSelect && g_pKeyManager->IsOnceKeyDown(VK_UP))
	{
		m_fSpeed++;
	}

	if (m_bSelect && g_pKeyManager->IsOnceKeyDown(VK_DOWN))
	{
		m_fSpeed--;
	}
}
