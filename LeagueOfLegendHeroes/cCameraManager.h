#pragma once

#define g_pCameraManager cCameraManager::GetInstance()

class cCamera;

class cCameraManager
{
	SINGLETON(cCameraManager);

public:
	enum eMoveType
	{
		CAMERAMOVETYPE_LERP, // 선형 보간
		CAMERAMOVETYPE_RERP, // 비율 보간 : 남아있는 양의 일정 퍼센트만큼
		CAMERAMOVETYPE_END
	};

	SYNTHESIZE(eMoveType, m_eMoveType, MoveType);
	SYNTHESIZE(D3DXVECTOR3, m_vCurPos, CurPos);
	SYNTHESIZE(D3DXVECTOR3*, m_pvTargetPos, TargetPosPtr);

private:
	
	D3DXVECTOR3 m_vRotation;
	float m_fDistanceFromTarget;

	D3DXVECTOR3 m_vLerpTargetPos;
	D3DXVECTOR3	m_vLerpDistance;
	D3DXVECTOR3 m_vLerpRotation;

	float m_vLerpTotalTime;
	float m_vLerpStartTime;

public:
	void Destroy() {};
	void Update();
	void StartLerpChase(D3DXVECTOR3* TargetPosPtr, float DistanceFromTarget, D3DXVECTOR3 Rotation, float LerpTotalTime);
};



