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
	if (!g_pCamera->GetLocked()) return;

	switch (m_eMoveType)
	{
	case CAMERAMOVETYPE_LERP:
	{
		float ratio = (g_pTimeManager->GetWorldTime() - m_fLerpStartTime) / m_fLerpTotalTime;

		if (ratio>=1) // 추적 끝
		{
			g_pCamera->SetTarget(m_pvTargetPos);
			g_pCamera->SetLocked(false);
		}
		else //추적중
		{
			D3DXVECTOR3 lerpPos = ratio*(*m_pvTargetPos) +(1-ratio)*(m_vLerpTargetPos);
			D3DXVECTOR3 lerpRot = ratio*m_vRotation + (1-ratio)*m_vLerpRotation;
			float		lerpDist = ratio*m_fDistanceFromTarget + (1 - ratio)*m_fLerpDistance;

			D3DXVECTOR3 relativePos;
			D3DXMATRIXA16 rotX, rotY;
			relativePos.x = 0.0f;
			relativePos.y = 0.0f;
			relativePos.z = -lerpDist;

			D3DXMatrixRotationX(&rotX, lerpRot.x);
			D3DXMatrixRotationY(&rotY, lerpRot.y);
			D3DXVec3TransformCoord(&relativePos, &relativePos, &(rotX*rotY));

			m_vCurPos = lerpPos + relativePos;
		}
	}
	break;
	case CAMERAMOVETYPE_SQUAR:
	{
		float ratio = (g_pTimeManager->GetWorldTime() - m_fLerpStartTime) / m_fLerpTotalTime;
		
		if (ratio >= 1) // 추적 끝
		{
			g_pCamera->SetTarget(m_pvTargetPos);
			g_pCamera->SetLocked(false);
		}
		else //추적중
		{
			ratio = (g_pTimeManager->GetWorldTime() - m_fLerpStartTime-m_fLerpTotalTime) / m_fLerpTotalTime;
			ratio *= ratio;
			ratio = 1 - ratio;

			D3DXVECTOR3 lerpPos		= ratio*(*m_pvTargetPos) + (1 - ratio)*(m_vLerpTargetPos);
			D3DXVECTOR3 lerpRot		= ratio*m_vRotation + (1 - ratio)*m_vLerpRotation;
			float		lerpDist	= ratio*m_fDistanceFromTarget + (1 - ratio)*m_fLerpDistance;

			D3DXVECTOR3 relativePos;
			D3DXMATRIXA16 rotX, rotY;
			relativePos.x = 0.0f;
			relativePos.y = 0.0f;
			relativePos.z = -lerpDist;

			D3DXMatrixRotationX(&rotX, lerpRot.x);
			D3DXMatrixRotationY(&rotY, lerpRot.y);
			D3DXVec3TransformCoord(&relativePos, &relativePos, &(rotX*rotY));

			m_vCurPos = lerpPos + relativePos;
		}
	}
	break;
	case CAMERAMOVETYPE_RERP:
	{
		if (fabs(m_fDistanceFromTarget-m_fLerpDistance)<RERP_END) // 추적 끝
		{
			g_pCamera->SetTarget(m_pvTargetPos);
			g_pCamera->SetLocked(false);
		}
		else //추적중
		{
			m_vLerpTargetPos = m_vLerpTargetPos + (*m_pvTargetPos - m_vLerpTargetPos)*m_fRerpRatio;
			m_vLerpRotation = m_vLerpRotation + (m_vRotation - m_vLerpRotation)*m_fRerpRatio;
			m_fLerpDistance = m_fLerpDistance + (m_fDistanceFromTarget - m_fLerpDistance)*m_fRerpRatio;
			
			D3DXVECTOR3 relativePos;
			D3DXMATRIXA16 rotX, rotY;
			relativePos.x = 0.0f;
			relativePos.y = 0.0f;
			relativePos.z = -m_fLerpDistance;

			D3DXMatrixRotationX(&rotX, m_vLerpRotation.x);
			D3DXMatrixRotationY(&rotY, m_vLerpRotation.y);
			D3DXVec3TransformCoord(&relativePos, &relativePos, &(rotX*rotY));

			m_vCurPos = m_vLerpTargetPos + relativePos;
		}
	}
	break;
	}
}

void cCameraManager::StartLerpChase(eMoveType MoveType, D3DXVECTOR3 * TargetPosPtr, float DistanceFromTarget, D3DXVECTOR3 Rotation, float LerpTotalTime)
{
	if (!TargetPosPtr) return;
	
	g_pCamera->SetLocked(true);

	m_eMoveType = MoveType;
	m_pvTargetPos = TargetPosPtr;
	m_fDistanceFromTarget = DistanceFromTarget;
	m_vRotation = Rotation;
	
	m_vLerpTargetPos = *g_pCamera->GetTarget();
	m_fLerpDistance = g_pCamera->GetDistanceFromTarget();
	m_vLerpRotation = g_pCamera->GetRotation();;

	m_fLerpStartTime = g_pTimeManager->GetWorldTime();
	m_fLerpTotalTime = LerpTotalTime;
	m_fRerpRatio = LerpTotalTime;
}
