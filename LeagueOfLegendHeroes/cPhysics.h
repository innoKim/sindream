#pragma once

#define PHYSICS_GRAVITY -1000.0f

class cPhysics
{
private:
	bool m_bStatic; //�浹�� ����� ������ �������� �ʴ� �����ȳ� (������ ����) ex) �ٴ�, ��
	
	SYNTHESIZE(bool, m_bIsActivate, IsActivate);
	SYNTHESIZE(float, m_fRestDuration, RestDuration);		//������ �浹�κ����� �ð�;

	SYNTHESIZE_PASS_BY_REF(float, m_fMass, Mass);			//����
	SYNTHESIZE_PASS_BY_REF(float, m_fRadius, Radius); // ����
	SYNTHESIZE_PASS_BY_REF(float, m_vElasticFactor, Elastic);		//ź����� 0~1.0f; 1.0f=��ź�浹.

	SYNTHESIZE(D3DXVECTOR3*, m_pvPos, PositionPtr);
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vAcceleration,	Acceleration);	//���ӵ�
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vVelocity,		Velocity);		//�ӵ�
	
	SYNTHESIZE(D3DXVECTOR3*, m_pvDir, DirectionPtr);
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vAngularVelocity, AngularVelocity);

public:
	cPhysics();
	virtual ~cPhysics();

	void Setup();
	void Update(float deltaTime);
	bool ColisionWith(set<cPhysics*> setVersusObject);

	float GetVelocityScalar() { return D3DXVec3Length(&m_vVelocity); }
};

