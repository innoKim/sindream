#include "stdafx.h"
#include "cBuilding.h"
#include "cPhysics.h"
#include "cMap.h"

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
	
	if (m_bSelect)
	{
		KeyControl();
	}
}

void cBuilding::KeyControl()
{
	RayInfo stRay = RayToScreenPoint(g_ptMouse.x, g_ptMouse.y);
	HitInfo stHit;

	bool temp = false;

	for (int x = 0; x < MAP_GRID; x++)
	{
		for (int z = 0; z < MAP_GRID; z++)
		{
			if (RayCast(stRay, stHit, m_pMap->GetHeightNode()[x][z].vecVertex))
			{
				m_vPos = stHit.hitpos;
				temp = true;
				break;
			}
		}
	}

	if (!temp)
	{
		RayInfo stRay2 = RayToScreenPoint(g_ptMouse.x, g_ptMouse.y);
		while (true)
		{
			if (stRay2.dir.y > 0) break;

			if (stRay2.pos.y < INFH) break;

			if (fabs(m_vPos.y - stRay2.pos.y)<1.0f)
			{
				m_vPos.x = stRay2.pos.x;
				m_vPos.z = stRay2.pos.z;
				break;
			}
			stRay2.pos += stRay2.dir;
		}
	}
		
	if (g_pKeyManager->IsStayKeyDown('Q'))
	{
		D3DXMATRIXA16 matR;
		D3DXMatrixRotationY(&matR, -0.01f);
		D3DXVec3TransformNormal(&m_vDir, &m_vDir, &matR);
	}

	if (g_pKeyManager->IsStayKeyDown('E'))
	{
		D3DXMATRIXA16 matR;
		D3DXMatrixRotationY(&matR,  0.01f);
		D3DXVec3TransformNormal(&m_vDir, &m_vDir, &matR);
	}
}
