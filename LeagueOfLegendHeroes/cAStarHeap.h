#pragma once

class cAStarNode;

class cAStarHeap
{
private:
	vector<cAStarNode*>	m_vecNode;

public:
	cAStarHeap();
	~cAStarHeap();

	void		AddNode(cAStarNode* pNode);
	cAStarNode*	GetMin();
	void		DeleteMin();
	void		Clear();

private:
	void Swap(int nIndex1, int nIndex2);
};

