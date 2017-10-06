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
	//만약 다 움직였으면 꺼져!
	if (m_pvTargetPos == g_pCamera->GetTarget()) return;

	D3DXVECTOR3 curPos, targetPos;
	curPos = *g_pCamera->GetTarget();
	targetPos = *m_pvTargetPos;

	if (D3DXVec3Length(&(curPos - targetPos)) < FLT_EPSILON) // 추적 끝
	{
		g_pCamera->SetTarget(m_pvTargetPos);
	}
	else // 추적해간다
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
