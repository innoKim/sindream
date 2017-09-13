#pragma once

#define PHYSICS_GRAVITY 0.01f
//
//struct ST_Contact
//{
//	ST_Contact() : Pn(0.0f), Pt(0.0f), Pnb(0.0f) {}
//
//	D3DXVECTOR3 position;
//	D3DXVECTOR3 normal;
//	D3DXVECTOR3 r1, r2;
//	float separation;
//	float Pn;	// accumulated normal impulse
//	float Pt;	// accumulated tangent impulse
//	float Pnb;	// accumulated normal impulse for position bias
//	float massNormal, massTangent;
//};

class cPhysics
{
private:
	bool m_bStatic; //�浹�� ����� ������ �������� �ʴ� �����ȳ� (������ ����) ex) �ٴ�, ��

	SYNTHESIZE_PASS_BY_REF(float, m_fRadius, Radius); // ����

	SYNTHESIZE_PASS_BY_REF(float,		m_fMass,			Mass); // ����
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vAcceleration,	Acceleration); //���ӵ�
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vVelocity,		Velovity);	//�ӵ�

	SYNTHESIZE(D3DXVECTOR3*,	m_pvPos,			PositionPtr);
	SYNTHESIZE(float,			m_fRestDuration,	RestDuration);		//������ �浹�κ����� �ð�;

	void SphereVsSphere(cPhysics * Object1, cPhysics * Object2);
	void SphereVsGround();

public:
	cPhysics();
	virtual ~cPhysics();

	void Setup();
	void Update();
	bool ColisionWith(vector<cPhysics*> vecVersusObject);
};

