#pragma once
class cParticle
{
	SYNTHESIZE(D3DXVECTOR3*,m_pvPosition,		PositionPtr);

	SYNTHESIZE(float,		m_fLifeTime,		LifeTime);
	SYNTHESIZE(float,		m_fCurTime,			CurTime);
	
	SYNTHESIZE(D3DXVECTOR3, m_vAcceleration,	Acceleration);
	SYNTHESIZE(D3DXVECTOR3, m_vVelocity,		Velocity);
	
	SYNTHESIZE(D3DXCOLOR,	m_dStartColor,		StartColor);
	SYNTHESIZE(D3DXCOLOR,	m_dEndColor,		EndColor);

	SYNTHESIZE(float,		m_fDragVelocity,	DragVelocity);	//저항력 ex 공기저항
	
public:
	cParticle();
	~cParticle();

	void Setup(D3DXVECTOR3 * PositionPtr, D3DXVECTOR3 Velocity, D3DXVECTOR3 Acceleration, float LifeTime, D3DXCOLOR StartColor, D3DXCOLOR EndColor, float DragVelocity);
	void Update();

	void AddAcc(D3DXVECTOR3 Acc) { m_vAcceleration += Acc; }
	void AddVelocity(D3DXVECTOR3 Velocity) { m_vVelocity += Velocity; }
	D3DXCOLOR GetCurColor();

	bool IsDead() { return ((m_fCurTime > m_fLifeTime) ? true : false); }
};

