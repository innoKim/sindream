#pragma once

class cAStarNode;
class cMap;

class cAStarGrid
{
private:
	cMap*						m_pMap;
	int							m_nSize;
	vector<vector<cAStarNode*>>	m_vecNode;

public:
	cAStarGrid();
	~cAStarGrid();

	void						Setup(cMap* pMap, int nNumLine = 150, int nSize = 50);
	cAStarNode*					GetNode(int x, int z);
	void						GetIndex(D3DXVECTOR3 vPos, OUT POINT* pt);
	vector<vector<cAStarNode*>> GetNodes() { return m_vecNode; }
};

