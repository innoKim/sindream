#pragma once

#define PHYSICS_GRAVITY -1000.0f

class cMap;

class cPhysics
{
private:
	bool m_bStatic; //충돌의 대상이 되지만 움직이지 않는 고정된놈 (무한의 질량) ex) 바닥, 벽
	
	SYNTHESIZE(bool, m_bIsActivate, IsActivate);
	SYNTHESIZE(float, m_fRestDuration, RestDuration);		//마지막 충돌로부터의 시간;

	SYNTHESIZE_PASS_BY_REF(float, m_fMass, Mass);			//질량
	SYNTHESIZE_PASS_BY_REF(float, m_fRadius, Radius); // 지름
	SYNTHESIZE_PASS_BY_REF(float, m_vElasticFactor, Elastic);		//탄성계수 0~1.0f; 1.0f=완탄충돌.

	SYNTHESIZE(D3DXVECTOR3*, m_pvPos, PositionPtr);
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vAcceleration,	Acceleration);	//가속도
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vVelocity,		Velocity);		//속도
	
	SYNTHESIZE(D3DXVECTOR3*, m_pvDir, DirectionPtr);
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vAngularVelocity, AngularVelocity);

	SYNTHESIZE(cMap*, m_pMap, MapPtr);

public:
	cPhysics();
	virtual ~cPhysics();

	void Setup();
	void Update(float deltaTime);
	bool ColisionWith(set<cPhysics*> setVersusObject);

	float GetVelocityScalar() { return D3DXVec3Length(&m_vVelocity); }

	float Height();
};

