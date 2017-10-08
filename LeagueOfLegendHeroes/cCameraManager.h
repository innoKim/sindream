#pragma once

#define g_pCameraManager cCameraManager::GetInstance()
#define RERP_END 0.1f
class cCamera;

class cCameraManager
{
	SINGLETON(cCameraManager);

public:
	enum eMoveType
	{
		CAMERAMOVETYPE_LERP, // 선형 보간
		CAMERAMOVETYPE_SQUAR, // 비율 보간 : 남아있는 양의 일정 퍼센트만큼
		CAMERAMOVETYPE_RERP,		
		CAMERAMOVETYPE_END
	};

	SYNTHESIZE(eMoveType, m_eMoveType, MoveType);
	SYNTHESIZE(D3DXVECTOR3, m_vCurPos, CurPos);
	SYNTHESIZE(D3DXVECTOR3*, m_pvTargetPos, TargetPosPtr);

private:
	
	D3DXVECTOR3 m_vRotation;
	float m_fDistanceFromTarget;

	D3DXVECTOR3 m_vLerpTargetPos;
	float		m_fLerpDistance;
	D3DXVECTOR3 m_vLerpRotation;


	float m_fLerpTotalTime;
	float m_fLerpStartTime;
	float m_fRerpRatio;

public:
	void Destroy() {};
	void Update();
	void StartLerpChase(eMoveType MoveType, D3DXVECTOR3* TargetPosPtr, float DistanceFromTarget, D3DXVECTOR3 Rotation, float LerpTotalTimeRerpRatio);
};



