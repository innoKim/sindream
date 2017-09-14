#include "stdafx.h"
#include "cMap.h"

cMap::cMap()
{
}


cMap::~cMap()
{
	for each (auto p in m_vecMap)
	{
		SAFE_DELETE(p);
	}
}

void cMap::LoadMap(IN char * Folder, IN char * File)
{
	cOBJLoader objLoader;
	objLoader.Load(Folder, File, m_vecMap);
}

void cMap::LoadSur(char * FileFullPath)
{
	cOBJLoader surLoader;
	surLoader.LoadSur(FileFullPath, m_vecSur, m_vecGrid);
}

float cMap::GetGroundHeight(D3DXVECTOR3 pos)
{
	m_stRay = Ray(D3DXVECTOR3(pos.x, 1000.f, pos.z), pos);
	
	if (RayCastPC(m_stRay, m_stHit, &m_vecSur))
	{
		return 1000.f - m_stHit.dist;
	}
	int a = 0;
	return 0.f;
}

void cMap::Render()
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);

	D3DXMatrixIdentity(&m_matW);

	// Supface 출력 부분
	//g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matW);
	//
	//g_pD3DDevice->SetTexture(0, NULL);
	//g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
	//g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vecSur.size() / 3, &m_vecSur[0], sizeof(ST_PC_VERTEX));
	//g_pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST, m_vecGrid.size() / 2, &m_vecGrid[0], sizeof(ST_PC_VERTEX));

	D3DXMatrixRotationX(&m_matR, -D3DX_PI / 2);

	m_matW *= m_matR;

	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matW);

	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	g_pShaderManager->SetMap(&m_vecMap, (D3DXMATRIXA16)m_matW);

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
}
