#pragma once

class cCamera;

class cCameraManager
{
public:
	
	enum eMoveType
	{
		CAMERAMOVETYPE_LERP, // ���� ����
		CAMERAMOVETYPE_RERP, // ���� ���� : �����ִ� ���� ���� �ۼ�Ʈ��ŭ
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



