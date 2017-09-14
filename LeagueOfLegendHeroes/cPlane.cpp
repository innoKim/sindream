#include "stdafx.h"
#include "cPlane.h"


cPlane::cPlane():
	m_pMesh(NULL)
{
}

cPlane::~cPlane()
{
	SAFE_RELEASE(m_pMesh);
}

void cPlane::Setup(int size)
{
	int nLine = size / 2;

	D3DXCreateMeshFVF(2,4, D3DXMESH_MANAGED, ST_PN_VERTEX::FVF, g_pD3DDevice, &m_pMesh);
	
	ST_PN_VERTEX* vb = NULL;
	m_pMesh->LockVertexBuffer(0, (LPVOID*)&vb);
	vb[0] = { D3DXVECTOR3(-nLine,0, nLine),D3DXVECTOR3(0,1,0) };
	vb[1] = { D3DXVECTOR3( nLine,0, nLine),D3DXVECTOR3(0,1,0) };
	vb[2] = { D3DXVECTOR3(-nLine,0,-nLine),D3DXVECTOR3(0,1,0) };
	vb[3] = { D3DXVECTOR3( nLine,0,-nLine),D3DXVECTOR3(0,1,0) };
	m_pMesh->UnlockVertexBuffer();

	WORD* ib = NULL;
	m_pMesh->LockIndexBuffer(0, (LPVOID*)&ib);
	ib[0] = 0;
	ib[1] = 1;
	ib[2] = 2;
	ib[3] = 2;
	ib[4] = 1;
	ib[5] = 3;
	m_pMesh->UnlockIndexBuffer();

	for (int i = 0; i < 6; i++)
	{
		m_vecVertex.push_back(vb[ib[i]].p);
	}

	vector<DWORD> adjagencyInfo(m_pMesh->GetNumFaces() * 3);
	m_pMesh->GenerateAdjacency(0.0f, &adjagencyInfo[0]);

	m_pMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
		&adjagencyInfo[0], NULL, 0, 0);


	ZeroMemory(&m_material, sizeof(D3DMATERIAL9));
	m_material.Ambient = m_material.Diffuse, m_material.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);

	g_pShaderManager->SetPlane(m_pMesh, matWorld);
}
