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
	if (m_pvTargetPos == g_pCamera->GetTarget()) return;

	D3DXVECTOR3 curPos, targetPos;
	curPos = *g_pCamera->GetTarget();
	targetPos = *m_pvTargetPos;

	if (D3DXVec3Length(&(curPos - targetPos)) < FLT_EPSILON) // ���� ��
	{
		g_pCamera->SetTarget(m_pvTargetPos);
	}
	else // �����ذ���
	{
		switch (m_eMoveType)
		{
		case CAMERAMOVETYPE_LERP:
			
			break;
		case CAMERAMOVETYPE_RERP:

			break;
		default:

			break;
		}
	}
}

void cCameraManager::ChangeState(eMoveType MoveType, D3DXVECTOR3 * TargetPosPtr, float DistanceFromTarget, D3DXVECTOR3 Rotation)
{

}
