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

	if (g_pKeyManager->IsOnceKeyDown(VK_SPACE))
	{
		m_vecPath = m_pAStar->GetPath(m_vPos, vPlayerPos);

		m_bTargetMove = true;

		m_vTargetPos = m_vPos;
	}

	if (m_bTargetMove)
	{
		float fDistance = D3DXVec3Length(&(m_vTargetPos - m_vPos));
		
		if (fDistance < ENEMY_SPD)
		{
			if (m_vecPath.size() > 0)
			{
				m_vPos = m_vTargetPos;

				m_vTargetPos = m_vecPath.front()->GetPosition();
				m_vecPath.erase(m_vecPath.begin());

				m_vTargetPos.y = m_pMap->GetHeight(m_vTargetPos);
			}
			else
			{
				m_bTargetMove = false;
			}
		}

		float fAngle = MY_UTIL::GetAngle(m_vTargetPos.x, m_vTargetPos.z, m_vPos.x, m_vPos.z);

		if (isnan(fAngle)) return;

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
