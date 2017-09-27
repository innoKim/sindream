#include "stdafx.h"
#include "cAStarNode.h"


cAStarNode::cAStarNode()
	: m_pParent(NULL)
	, m_fFCost(0.0f)
	, m_fGCost(0.0f)
	, m_fHCost(0.0f)
	, m_vPos(D3DXVECTOR3(0, 0, 0))
	, m_isOpen(false)
{
}


cAStarNode::~cAStarNode()
{
	m_pParent = NULL;
}

POINT cAStarNode::GetIndex()
{
	POINT pt;
	pt.x = m_vPos.x / m_nSize;
	pt.y = m_vPos.z / m_nSize;

	return pt;
}
