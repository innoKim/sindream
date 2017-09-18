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
	SYNTHESIZE_PASS_BY_REF(float,	m_fRadius,			Radius);					// ����
	SYNTHESIZE_PASS_BY_REF(float,	m_vElasticFactor,	Elastic);					//ź����� 0~1.0f; 1.0f=��ź�浹.
	SYNTHESIZE(bool,				m_bIsActivate,		IsActivate);				//���������� �������� �����Ұ��ΰ�
	SYNTHESIZE(float,				m_fRestDuration,	RestDuration);				//������ �浹�κ����� �ð�;
	
	//�����
	SYNTHESIZE_PASS_BY_REF(float,		m_fMass,			Mass);					//����
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vVelocity,		Velocity);				//�ӵ�
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vAcceleration,	Acceleration);			//���ӵ�
	SYNTHESIZE_PASS_BY_REF(ST_FORCE,	m_stForce,			Force);					//�ۿ��ϴ� ��

	//ȸ���
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vRotationInertia, RotationInertia);		//��������
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vAngularVelocity,		AngularVelocity);
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vAngularAcceleration, AngularAcceleration);
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vTorque,				Torque);			//��ũ
	
public:
	cRigidbody();
	virtual ~cRigidbody();
	
	void Update(float deltaTime);

	//�̹������� �浹�� ����� �˻� //�浹������ �浹 �Ƚ�ų����
	bool IsCollidedAtThisLoop() { return (m_fRestDuration>g_pTimeManager->GetElapsedTime()) ? false : true; }
	void ResetRestDuration() { m_fRestDuration = 0.0f; }
};

