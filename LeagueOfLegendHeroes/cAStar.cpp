#include "stdafx.h"
#include "cAStar.h"
#include "cAStarGrid.h"
#include "cAStarNode.h"
#include "cAstarHeap.h"

cAStar::cAStar()
	: m_pAstarGrid(NULL)
	, m_pStartNode(NULL)
	, m_pEndNode(NULL)
	, m_pOpenHeap(NULL)
{
}


cAStar::~cAStar()
{
	SAFE_DELETE(m_pOpenHeap);
	m_pStartNode = NULL;
	m_pEndNode = NULL;
	m_pAstarGrid = NULL;
}

void cAStar::Setup(cAStarGrid * pAstarGrid)
{
	m_pAstarGrid = pAstarGrid;

	m_vecNode = pAstarGrid->GetNodes();

	m_pOpenHeap = new cAStarHeap;
}

vector<cAStarNode*> cAStar::GetPath(D3DXVECTOR3 vStartPos, D3DXVECTOR3 vEndPos)
{
	POINT ptStart;

	m_pAstarGrid->GetIndex(vStartPos, &ptStart);

	POINT ptEnd;

	m_pAstarGrid->GetIndex(vEndPos, &ptEnd);

	m_pStartNode = m_pAstarGrid->GetNode(ptStart.x, ptStart.y);
	m_pEndNode = m_pAstarGrid->GetNode(ptEnd.x, ptEnd.y);

	vector<cAStarNode*> m_vecClosedList;

	if (m_pStartNode && m_pEndNode)
	{
		m_pStartNode->SetHCost(abs(ptStart.x - ptEnd.x) + abs(ptStart.y - ptEnd.y));
		m_pStartNode->SetGCost(0);
		m_pStartNode->SetFCost(m_pStartNode->GetGCost() + m_pStartNode->GetHCost());
		m_pStartNode->SetIsOpen(true);
		m_pOpenHeap->AddNode(m_pStartNode);

		while (true)
		{
			cAStarNode* pCurrentNode = m_pOpenHeap->GetMin();
	
			if (pCurrentNode)
			{
				m_pOpenHeap->DeleteMin();
				pCurrentNode->SetIsOpen(false);
				m_vecClosedList.push_back(pCurrentNode);
			}
			else
			{
				m_pOpenHeap->Clear();
				break;
			}

			if (pCurrentNode == m_pEndNode)
			{
				m_pOpenHeap->Clear();
				return RetracePath();
			}

			vector<cAStarNode*> neighbours;

			cAStarNode *tempNode = pCurrentNode;

			for (int i = -1; i <= 1; i++)
			{
				for (int j = -1; j <= 1; j++)
				{
					int gridX = pCurrentNode->GetIndex().x;
					int gridY = pCurrentNode->GetIndex().y;

					if (i == 0 && j == 0) continue;
					if (gridX == 0 && j == -1) continue;
					if (gridY == 0 && i == -1) continue;

					if (gridX == m_vecNode.size() - 1 && j == 1) continue;
					if (gridY == m_vecNode.size() - 1 && i == 1) continue;

					tempNode = m_vecNode[gridY + i][gridX + j];

					m_iter = std::find(m_vecClosedList.begin(), m_vecClosedList.end(), tempNode);
					if (!m_pAstarGrid->GetNode(gridX + j, gridY + i) || m_iter != m_vecClosedList.end()) continue;

					neighbours.push_back(tempNode);
				}
			}

			for (int i = 0; i < neighbours.size(); i++)
			{
				int costNeighbours = pCurrentNode->GetGCost() + GetGDistance(pCurrentNode, neighbours[i]);

				if (costNeighbours < neighbours[i]->GetGCost() || !neighbours[i]->GetIsOpen())
				{
					neighbours[i]->SetGCost(costNeighbours);
					neighbours[i]->SetHCost(GetHDistance(m_pEndNode, neighbours[i]));
					neighbours[i]->SetFCost(neighbours[i]->GetGCost() + neighbours[i]->GetHCost());

					neighbours[i]->SetParent(pCurrentNode);
					
					neighbours[i]->SetIsOpen(true);
					m_pOpenHeap->AddNode(neighbours[i]);
				}
			}
		}
	}

	return m_vecPath;
}

vector<cAStarNode*> cAStar::RetracePath()
{
	cAStarNode* pTempNode = m_pEndNode;
	vector<cAStarNode*> vecPath;

	while (pTempNode != m_pStartNode)
	{
		m_vecPath.push_back(pTempNode);
		pTempNode = pTempNode->GetParent();
	}

	while (m_vecPath.size() > 0)
	{
		vecPath.push_back(m_vecPath.back());
		m_vecPath.pop_back();
	}

	return vecPath;
}

float cAStar::GetGDistance(cAStarNode * pNode1, cAStarNode * pNode2)
{
	POINT ptNode1 = pNode1->GetIndex();
	POINT ptNode2 = pNode2->GetIndex();

	int dstX = abs(ptNode1.x - ptNode2.x);
	int dstY = abs(ptNode1.y - ptNode2.y);

	if (dstX > dstY) return 1.4f * dstY + 1.0f * (dstX - dstY);
	else return 1.4f * dstX + 1.0f * (dstY - dstX);
}

float cAStar::GetHDistance(cAStarNode * pNode1, cAStarNode * pNode2)
{
	POINT ptNode1 = pNode1->GetIndex();
	POINT ptNode2 = pNode2->GetIndex();

	int dstX = abs(ptNode1.x - ptNode2.x);
	int dstY = abs(ptNode1.y - ptNode2.y);

	return dstX + dstY;
}
