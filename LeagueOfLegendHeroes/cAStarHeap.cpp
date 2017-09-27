#include "stdafx.h"
#include "cAStarHeap.h"
#include "cAstarNode.h"


cAStarHeap::cAStarHeap()
{
}


cAStarHeap::~cAStarHeap()
{
}

void cAStarHeap::AddNode(cAStarNode * pNode)
{
	m_vecNode.push_back(pNode);

	int nIndex = m_vecNode.size() - 1;

	while (nIndex > 0)
	{
		int nParentIndex = (nIndex - 1) / 2;

		if (m_vecNode[nIndex]->GetFCost() < m_vecNode[nParentIndex]->GetFCost())
		{
			Swap(nIndex, nParentIndex);
		}
		else
		{
			break;
		}

		nIndex = nParentIndex;
	}
}

cAStarNode * cAStarHeap::GetMin()
{
	if (!m_vecNode.empty())
		return m_vecNode.front();
	else
		return NULL;
}

void cAStarHeap::DeleteMin()
{
	cAStarNode* pRoot = m_vecNode.front();

	int nIndex = 0;

	m_vecNode[nIndex] = m_vecNode.back();

	while (nIndex < m_vecNode.size())
	{
		int nLeftChildIndex = nIndex * 2 + 1;
		int nRightChildIndex = nIndex * 2 + 2;

		cAStarNode* pChild = NULL;
		int nChildIndex = 0;

		if (nLeftChildIndex > m_vecNode.size() - 1)
		{
			break;
		}
		else if (nRightChildIndex > m_vecNode.size() - 1)
		{
			pChild = m_vecNode[nLeftChildIndex];
			nChildIndex = nLeftChildIndex;
		}
		else
		{
			if (m_vecNode[nLeftChildIndex]->GetFCost() < m_vecNode[nRightChildIndex]->GetFCost())
			{
				pChild = m_vecNode[nLeftChildIndex];
				nChildIndex = nLeftChildIndex;
			}
			else
			{
				pChild = m_vecNode[nRightChildIndex];
				nChildIndex = nRightChildIndex;
			}
		}

		if (m_vecNode[nIndex]->GetFCost() > pChild->GetFCost())
		{
			Swap(nIndex, nChildIndex);
		}
		else
		{
			break;
		}

		nIndex = nChildIndex;
	}

	m_vecNode.pop_back();
}

void cAStarHeap::Clear()
{
	for each(auto p in m_vecNode)
	{
		p->SetIsOpen(false);
	}

	m_vecNode.clear();
}

void cAStarHeap::Swap(int nIndex1, int nIndex2)
{
	cAStarNode* pTemp = NULL;

	pTemp = m_vecNode[nIndex1];
	m_vecNode[nIndex1] = m_vecNode[nIndex2];
	m_vecNode[nIndex2] = pTemp;
}
