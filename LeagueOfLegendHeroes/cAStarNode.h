#pragma once

class cAStarNode
{
private:
	SYNTHESIZE(int,			m_nSize,	Size);
	SYNTHESIZE(D3DXVECTOR3,	m_vPos,		Position);
	SYNTHESIZE(float,		m_fFCost,	FCost);
	SYNTHESIZE(float,		m_fGCost,	GCost);
	SYNTHESIZE(float,		m_fHCost,	HCost);
	SYNTHESIZE(bool,		m_isOpen,	IsOpen);
	SYNTHESIZE(cAStarNode*, m_pParent,	Parent);

public:
	cAStarNode();
	~cAStarNode();

	POINT	GetIndex();
};

