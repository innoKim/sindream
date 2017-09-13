#include "stdafx.h"
#include "cPlayer.h"

cPlayer::cPlayer()
{
}

cPlayer::~cPlayer()
{
}

void cPlayer::Update()
{
	KeyControl();
	cUnit::Update();
}

void cPlayer::Render()
{
	cUnit::Render();
}

void cPlayer::KeyControl()
{
	FindMoveDir();

	if (abs(m_moveInfo.nHorizontalFactor) + abs(m_moveInfo.nVerticalFactor) > 0)
	{
		//Lerp Dir and MoveDir
		m_vDir = D3DXVECTOR3(m_vDir.x*0.9f + m_moveInfo.vMoveDir.x*0.1f, m_vDir.y, m_vDir.z*0.9f + m_moveInfo.vMoveDir.z*0.1f);
		m_vPos += PLAYER_SPD*m_vDir;
		SetState(STATE_RUN);
	}

	else if(m_pCurState == m_mapStates[STATE_RUN])
	{
		SetState(STATE_IDLE);
	}

	if (g_pKeyManager->IsOnceKeyDown('Q'))
	{
		if (IsReady()) SetState(STATE_SPELL1);
	}
	if (g_pKeyManager->IsOnceKeyDown('E'))
	{
		if (IsReady()) SetState(STATE_SPELL2);
	}
}

void cPlayer::FindMoveDir()
{
	m_moveInfo.vVerticalDir = g_pCamera->GetDirParrallelToPlane();

	D3DXMATRIX moveDirRotation;
	D3DXMatrixRotationY(&moveDirRotation, D3DX_PI / 2);
	
	D3DXVec3TransformNormal(&m_moveInfo.vHorizontalDir, &m_moveInfo.vVerticalDir, &moveDirRotation);

	m_moveInfo.nVerticalFactor = m_moveInfo.nHorizontalFactor = 0;

	if (g_pKeyManager->IsStayKeyDown('W') && IsReady()) m_moveInfo.nVerticalFactor++;
	if (g_pKeyManager->IsStayKeyDown('A') && IsReady()) m_moveInfo.nHorizontalFactor--;
	if (g_pKeyManager->IsStayKeyDown('S') && IsReady()) m_moveInfo.nVerticalFactor--;
	if (g_pKeyManager->IsStayKeyDown('D') && IsReady())
	{
		m_moveInfo.nHorizontalFactor++;
	}
	m_moveInfo.vMoveDir = m_moveInfo.nHorizontalFactor*m_moveInfo.vHorizontalDir + m_moveInfo.nVerticalFactor*m_moveInfo.vVerticalDir;
	D3DXVec3Normalize(&m_moveInfo.vMoveDir, &m_moveInfo.vMoveDir);
}
