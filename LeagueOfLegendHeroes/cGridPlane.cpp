#include "stdafx.h"
#include "cGridPlane.h"


cGridPlane::cGridPlane()
{
}


cGridPlane::~cGridPlane()
{
}

void cGridPlane::Setup(int size,int nodeSize)
{
	int nLine = size / 2;

	ST_PC_VERTEX v = { D3DXVECTOR3(nLine*nodeSize,	0,	0)	, D3DXCOLOR(255,0,0,1) };
	m_vecVertex.push_back(v);
	v = { D3DXVECTOR3(-nLine*nodeSize,0,	0)	 , D3DCOLOR_XRGB(255,0,0) };
	m_vecVertex.push_back(v);
	v = { D3DXVECTOR3(0,	0,nLine*nodeSize), D3DCOLOR_XRGB(0,0,255) };
	m_vecVertex.push_back(v);
	v = { D3DXVECTOR3(0,0,-nLine*nodeSize)	, D3DCOLOR_XRGB(0,0,255) };
	m_vecVertex.push_back(v);
	v = { D3DXVECTOR3(0,nLine*nodeSize,0)	, D3DCOLOR_XRGB(0,255,0) };
	m_vecVertex.push_back(v);
	v = { D3DXVECTOR3(0,-nLine*nodeSize,0)	, D3DCOLOR_XRGB(0,255,0) };
	m_vecVertex.push_back(v);

	for (int i = 1; i <= nLine; i++)
	{
		v = { D3DXVECTOR3(nLine*nodeSize,	0,	i*nodeSize)	, ((i % 5 == 0) ? D3DCOLOR_XRGB(255,255,255) : D3DCOLOR_XRGB(64,64,64)) };
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(-nLine*nodeSize, 0, i*nodeSize);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(nLine*nodeSize, 0, -i*nodeSize);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(-nLine*nodeSize, 0, -i*nodeSize);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(i*nodeSize, 0, nLine*nodeSize);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(i*nodeSize, 0, -nLine*nodeSize);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(-i*nodeSize, 0, nLine*nodeSize);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(-i*nodeSize, 0, -nLine*nodeSize);
		m_vecVertex.push_back(v);
	}
}

void cGridPlane::Render()
{
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
	g_pD3DDevice->SetTexture(0, NULL);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST, m_vecVertex.size() / 2, m_vecVertex.data(), sizeof(ST_PC_VERTEX));
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
}
