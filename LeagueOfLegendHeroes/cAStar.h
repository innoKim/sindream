#pragma once

class cAStarNode;
class cAStarGrid;
class cAStarHeap;

class cAStar
{
private:
	vector<cAStarNode*>::iterator	m_iter;
	vector<cAStarNode*>				m_vecPath;
	cAStarGrid*						m_pAstarGrid;
	cAStarNode*						m_pStartNode;
	cAStarNode*						m_pEndNode;
	vector<vector<cAStarNode*>>		m_vecNode;
	cAStarHeap*						m_pOpenHeap;

public:
	cAStar();
	~cAStar();

	void Setup(cAStarGrid* pAstarGrid);
	vector<cAStarNode*>	GetPath(D3DXVECTOR3 vStartPos, D3DXVECTOR3 vEndPos);
	vector<cAStarNode*> RetracePath();
	float GetGDistance(cAStarNode* pNode1, cAStarNode* pNode2);
	float GetHDistance(cAStarNode* pNode1, cAStarNode* pNode2);
};

