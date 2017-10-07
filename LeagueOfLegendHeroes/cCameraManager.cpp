#include "stdafx.h"
#include "cCameraManager.h"


cCameraManager::cCameraManager()
{
}


cCameraManager::~cCameraManager()
{
}

void cCameraManager::Update()
{
	//���� �� ���������� ����!
	if (!g_pCamera->GetLocked()) return;

	switch (m_eMoveType)
	{
	case CAMERAMOVETYPE_LERP:
	{
		float ratio = (g_pTimeManager->GetWorldTime() - m_vLerpStartTime) / m_vLerpTotalTime;

		if (ratio>=1) // ���� ��
		{
			g_pCamera->SetTarget(m_pvTargetPos);
			g_pCamera->SetLocked(false);
		}
		else //������
		{
			
		}
	}
		break;
	}
}

void cCameraManager::StartLerpChase(D3DXVECTOR3 * TargetPosPtr, float DistanceFromTarget, D3DXVECTOR3 Rotation, float LerpTotalTime)
{
	if (!TargetPosPtr) return;

	g_pCamera->SetLocked(true);

	m_pvTargetPos = TargetPosPtr;
	m_fDistanceFromTarget = DistanceFromTarget;
	m_vRotation = Rotation;
	
	m_vLerpTargetPos = *g_pCamera->GetTarget();
	m_vLerpStartTime = g_pTimeManager->GetWorldTime();
	m_vLerpTotalTime = LerpTotalTime;
}
