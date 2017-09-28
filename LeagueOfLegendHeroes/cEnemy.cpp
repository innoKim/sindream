#include "stdafx.h"
#include "cEnemy.h"
#include "cAStar.h"
#include "cMap.h"
#include "cAStarNode.h"

cEnemy::cEnemy()
	: m_pAStar(NULL)
	, m_pAStarGrid(NULL)
	, m_bTargetMove(false)
	, m_fWorldTime(0.0f)
{
}


cEnemy::~cEnemy()
{
	m_pAStarGrid = NULL;

	SAFE_DELETE(m_pAStar);

	Destroy();
}

void cEnemy::Setup(vector<ST_UNITLOADINFO> statesVector, cMap * mapPtr)
{
	cUnit::Setup(statesVector, mapPtr);

	m_pAStarGrid = m_pMap->GetGrid();

	m_pAStar = new cAStar;
	m_pAStar->Setup(m_pAStarGrid);
}

void cEnemy::Update(D3DXVECTOR3 vPlayerPos)
{
	cUnit::Update();

	m_vTargetPos = vPlayerPos;

	if (m_fWorldTime + 1.0f < g_pTimeManager->GetWorldTime())
	{
		m_fWorldTime = g_pTimeManager->GetWorldTime();

		m_vecPath = m_pAStar->GetPath(m_vPos, vPlayerPos);

		if (m_vecPath.size() > 0)
		{
			m_vTargetPos = m_vecPath.front()->GetPosition();
			m_vecPath.erase(m_vecPath.begin());
			m_vTargetPos.y = m_pMap->GetHeight(m_vTargetPos);

			m_bTargetMove = true;
		}
	}

	if (m_bTargetMove)
	{
		float fAngle = MY_UTIL::GetAngle(vPlayerPos.x, vPlayerPos.z, m_vPos.x, m_vPos.z);

		m_vDir = D3DXVECTOR3(-1, 0, 0);
		D3DXMATRIX matR;
		D3DXMatrixRotationY(&matR, fAngle);
		D3DXVec3TransformNormal(&m_vDir, &m_vDir, &matR);

		m_vPos = m_vPos + m_vDir * ENEMY_SPD;

		m_vPos.y = m_pMap->GetHeight(m_vPos);
	}
}

void cEnemy::Render()
{
	cUnit::Render();
}
