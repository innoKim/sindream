#pragma once

class cCamera;

class cCameraManager
{
public:
	
	enum eMoveType
	{
		CAMERAMOVETYPE_LERP, // 선형 보간
		CAMERAMOVETYPE_RERP, // 비율 보간 : 남아있는 양의 일정 퍼센트만큼
		CAMERAMOVETYPE_END
	};
	
	SINGLETON(cCameraManager);

private:
	SYNTHESIZE(eMoveType,		m_eMoveType,			MoveType);
	SYNTHESIZE(D3DXVECTOR3*,	m_pvTargetPos,			TargetPosPtr);
	
	SYNTHESIZE(float,			m_fDistanceFromTarget,	DistanceFromTarget);
	SYNTHESIZE(D3DXVECTOR3,		m_vRotation,			Rotation);
	
public:
	void Destroy();
	void Update();	
	void ChangeState(eMoveType MoveType, D3DXVECTOR3* TargetPosPtr, float DistanceFromTarget, D3DXVECTOR3 Rotation);
};



