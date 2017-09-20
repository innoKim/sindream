#pragma once
class cParticle
{
private:
	D3DXVECTOR3 m_vAcceleration;
	D3DXVECTOR3 m_vVelocity;
	D3DXCOLOR	m_dStartColor;
	D3DXCOLOR	m_dEndColor;

public:
	cParticle();
	~cParticle();

	void Setup();
	void Update();
};

