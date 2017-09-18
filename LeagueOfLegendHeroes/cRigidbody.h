#pragma once

struct ST_FORCE
{
	D3DXVECTOR3 force = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 pos = D3DXVECTOR3(0, 0, 0);

	void Reset() { force = D3DXVECTOR3(0, 0, 0); pos = D3DXVECTOR3(0, 0, 0); }
};

class cRigidbody
{

private:
	SYNTHESIZE(D3DXVECTOR3*,		m_pvPos,			PositionPtr);
	SYNTHESIZE(D3DXVECTOR3*,		m_pvDir,			DirectionPtr);
	SYNTHESIZE_PASS_BY_REF(float,	m_fRadius,			Radius);					// 지름
	SYNTHESIZE_PASS_BY_REF(float,	m_vElasticFactor,	Elastic);					//탄성계수 0~1.0f; 1.0f=완탄충돌.
	SYNTHESIZE(bool,				m_bIsActivate,		IsActivate);				//물리엔진의 움직임을 수행할것인가
	SYNTHESIZE(float,				m_fRestDuration,	RestDuration);				//마지막 충돌로부터의 시간;
	
	//선형운동
	SYNTHESIZE_PASS_BY_REF(float,		m_fMass,			Mass);					//질량
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vVelocity,		Velocity);				//속도
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vAcceleration,	Acceleration);			//가속도
	SYNTHESIZE_PASS_BY_REF(ST_FORCE,	m_stForce,			Force);					//작용하는 힘

	//회전운동
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vRotationInertia, RotationInertia);		//관성질량
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vAngularVelocity,		AngularVelocity);
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vAngularAcceleration, AngularAcceleration);
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vTorque,				Torque);			//토크
	
public:
	cRigidbody();
	virtual ~cRigidbody();
	
	void Update(float deltaTime);

	//이번루프에 충돌이 됬느냐 검사 //충돌됬으면 충돌 안시킬꺼임
	bool IsCollidedAtThisLoop() { return (m_fRestDuration>g_pTimeManager->GetElapsedTime()) ? false : true; }
	void ResetRestDuration() { m_fRestDuration = 0.0f; }
};

