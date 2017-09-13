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
	if (g_pKeyManager->IsStayKeyDown('W') && IsReady())
	{
		m_vPos += PLAYER_SPD*m_vDir;
		SetState(STATE_RUN);
	}
	else if (m_pCurState == m_mapStates[STATE_RUN])
	{
		SetState(STATE_IDLE);
	}
	if (g_pKeyManager->IsStayKeyDown('S'))
	{
		m_vPos -= PLAYER_SPD*m_vDir;
	}
	if (g_pKeyManager->IsStayKeyDown('A'))
	{
		D3DXMATRIX Rotation;
		D3DXMatrixRotationY(&Rotation, -PLAYER_ROTSPD);
		D3DXVec3TransformNormal(&m_vDir, &m_vDir, &Rotation);
	}
	if (g_pKeyManager->IsStayKeyDown('D'))
	{
		D3DXMATRIX Rotation;
		D3DXMatrixRotationY(&Rotation, PLAYER_ROTSPD);
		D3DXVec3TransformNormal(&m_vDir, &m_vDir, &Rotation);
	}

	if (g_pKeyManager->IsOnceKeyDown('Q'))
	{
		if (IsReady()) SetState(STATE_SPELL1);
	}
}
