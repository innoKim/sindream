#include "stdafx.h"
#include "cMap.h"
#include "cBuilding.h"
#include "cGroup.h"

cMap::cMap()
	: m_pMesh(NULL)
{
}


cMap::~cMap()
{
	for each (auto p in m_vecMap)
	{
		SAFE_DELETE(p);
	}

	for each (auto p in m_vecMtlTex)
	{
		SAFE_DELETE(p);
	}

	SAFE_RELEASE(m_pMesh);
}

void cMap::LoadMap(IN char * Folder, IN char * File)
{
	cOBJLoader objLoader;
	objLoader.Load(Folder, File, m_vecMap, m_mapMtlTex);

	D3DXMatrixIdentity(&m_matW);

	D3DXMatrixRotationX(&m_matR, -D3DX_PI / 2);

	m_matW *= m_matR;

	int nIndex = 0;

	for (int i = 0; i < m_vecMap.size(); i++)
	{
		nIndex += m_vecMap[i]->GetVertex().size();
	}

	D3DXCreateMeshFVF(nIndex / 3, nIndex, D3DXMESH_MANAGED | D3DXMESH_32BIT, ST_PT_VERTEX::FVF, g_pD3DDevice, &m_pMesh);

	ST_PT_VERTEX* pStVertex = new ST_PT_VERTEX[nIndex];
	DWORD* pNIndex = new DWORD[nIndex];

	m_pMesh->LockVertexBuffer(NULL, (LPVOID*)&pStVertex);
	m_pMesh->LockIndexBuffer(NULL, (LPVOID*)&pNIndex);

	int index = 0;

	for (int i = 0; i < m_vecMap.size(); i++)
	{
		for (int j = 0; j < m_vecMap[i]->GetVertex().size(); j++)
		{
			pStVertex[index] = (m_vecMap[i]->GetVertex())[j];
			pNIndex[index] = index;
			index++;
		}
	}

	m_pMesh->UnlockVertexBuffer();
	m_pMesh->UnlockIndexBuffer();

	m_vecMtlTex.resize(m_mapMtlTex.size());

	for (int i = 0; i < m_vecMap.size(); i++)
	{
		index = 0;
		for (m_iterMtlTex = m_mapMtlTex.begin(); m_iterMtlTex != m_mapMtlTex.end(); ++m_iterMtlTex)
		{
			if (m_iterMtlTex->second == m_vecMap[i]->GetMtlTex())
			{
				m_vecMtlTex[index] = m_vecMap[i]->GetMtlTex();
				break;
			}
			index++;
		}
		m_vecMap[i]->SetSubset(index);
	}

	DWORD* pSubsetBuffer = new DWORD[nIndex];
	m_pMesh->LockAttributeBuffer(NULL, &pSubsetBuffer);

	index = 0;
	for (int i = 0; i < m_vecMap.size(); i++)
	{
		for (int j = 0; j < m_vecMap[i]->GetVertex().size() / 3; j++)
		{
			pSubsetBuffer[index] = m_vecMap[i]->GetSubset();
			index++;
		}
	}

	m_pMesh->UnlockAttributeBuffer();

	vector<DWORD> vecAdj(m_pMesh->GetNumFaces() * 3);
	m_pMesh->GenerateAdjacency(0.0f, &vecAdj[0]);

	m_pMesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_COMPACT | D3DXMESHOPT_VERTEXCACHE, &vecAdj[0], NULL, NULL, NULL);

	g_pShaderManager->SetMap(m_pMesh, m_vecMtlTex, (D3DXMATRIXA16)m_matW);
}

void cMap::LoadSur(char * FileFullPath)
{
	cOBJLoader surLoader;
	surLoader.LoadSur(FileFullPath, m_vecSur, m_gridHeightNode);
}

float cMap::GetHeight(D3DXVECTOR3 pos)
{
	//RayInfo m_stRay = Ray(D3DXVECTOR3(pos.x, 1000.f, pos.z), pos);
	//HitInfo m_stHit;

	//if (RayCastPC(m_stRay, m_stHit, &m_vecSur))
	//{
	//	return 1000.f - m_stHit.dist;
	//}
	//int a = 0;
	//return 0.f;

	//먼저 대상노드를 찾고
	float width = MAP_SIZE * MAP_RATIO / MAP_GRID;

	int indexX = pos.x / width;
	int indexZ = pos.z / width;

	RayInfo	Ray(D3DXVECTOR3(pos.x, 1000.f, pos.z), D3DXVECTOR3(0, -1, 0));
	HitInfo	Hit;

	/*for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (indexX + i >= m_gridHeightNode.size() || indexX + i < 0) continue;
			if (indexZ + j >= m_gridHeightNode[indexX + i].size() || indexZ + j < 0) continue;

			if (RayCast(Ray, Hit, m_gridHeightNode[indexX+i][indexZ+j].vecVertex))
			{
				return 1000.f - Hit.dist;
			}
		}
	}*/
	if (RayCast(Ray, Hit, m_gridHeightNode[indexX][indexZ].vecVertex))
	{
		return 1000.f - Hit.dist;
	}

	return INFH;
}