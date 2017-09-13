#pragma once

#define PHYSICS_GRAVITY 0.01f

class cPhysics
{
private:
	bool m_bStatic; //�浹�� ����� ������ �������� �ʴ� �����ȳ� (������ ����) ex) �ٴ�, ��

	SYNTHESIZE_PASS_BY_REF(float,		m_fMass,			Mass); // ����
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vAcceleration,	Acceleration); //���ӵ�
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vVelocity,		Velovity);	//�ӵ�

	SYNTHESIZE(D3DXVECTOR3*, m_pvPos, PositionPtr);

	float m_fRestDuration; //������ �浹�κ����� �ð�;

public:
	cPhysics();
	virtual ~cPhysics();

	void Setup();
	void Update();
};

