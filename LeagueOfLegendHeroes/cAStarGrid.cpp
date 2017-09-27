#include "stdafx.h"
#include "cAStarGrid.h"
#include "cAStarNode.h"
#include "cMap.h"

cAStarGrid::cAStarGrid()
	: m_pMap(NULL)
{
}


cAStarGrid::~cAStarGrid()
{
	m_pMap = NULL;

	for (int i = 0; i < m_vecNode.size(); i++)
	{
		for (int j = 0; j < m_vecNode[i].size(); j++)
		{
			SAFE_DELETE(m_vecNode[i][j]);
		}
	}
}

void cAStarGrid::Setup(cMap* pMap, int nNumLine, int nSize)
{
	m_pMap = pMap;

	m_nSize = nSize;

	for (int z = 0; z < nNumLine; z++)
	{
		vector<cAStarNode*> vecNode;
		for (int x = 0; x < nNumLine; x++)
		{
			cAStarNode* pNode = new cAStarNode;
			pNode->SetSize(m_nSize);
			pNode->SetPosition(D3DXVECTOR3(x * m_nSize, 0, z * m_nSize));
			vecNode.push_back(pNode);
		}
		m_vecNode.push_back(vecNode);
	}
}

cAStarNode * cAStarGrid::GetNode(int x, int z)
{
	if (m_pMap->GetHeight(m_vecNode[z][x]->GetPosition()) != INFH)
	{
		return m_vecNode[z][x];
	}

	return NULL;
}

void cAStarGrid::GetIndex(D3DXVECTOR3 vPos, OUT POINT* pt)
{
	int nX = vPos.x;
	int nZ = vPos.z;

	float fX = vPos.x - nX;
	float fZ = vPos.z - nZ;

	if (fX > 0.5f)
	{
		pt->x = nX + 1;
	}
	else
	{
		pt->x = nX;
	}

	if (fZ > 0.5f)
	{
		pt->y = nZ + 1;
	}
	else
	{
		pt->y = nZ;
	}
}
