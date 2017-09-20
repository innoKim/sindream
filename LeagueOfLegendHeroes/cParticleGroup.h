#pragma once

class cParticle;

class cParticleGroup
{
private:
	vector<ST_PC_VERTEX>	m_vecVertex;
	vector<cParticle*>		m_vecParticle;
	
	SYNTHESIZE(string,		m_szTexturePath,			TexturePath);

	SYNTHESIZE(bool,		m_bIsContinuous,			IsContinueus);
	
	SYNTHESIZE(int,			m_nInitParticle,			InitParticleNumber);
	SYNTHESIZE(int,			m_nGenParticle,				GenParticleNumber);
	
	SYNTHESIZE(float,		m_fLifeTime,				LifeTime);
	SYNTHESIZE(float,		m_fLifeTimeVariation,		LifeTimeVariation);

	SYNTHESIZE(D3DXVECTOR3, m_vStartPosition,			StartPosition);
	SYNTHESIZE(float,		m_vStartPositionVariation,	StartPositionVariation);

	SYNTHESIZE(D3DXVECTOR3, m_vVelocity,				Velocity);
	SYNTHESIZE(float,		m_vVelocityVariation,		VelocityVariation);
	SYNTHESIZE(float,		m_fDragVelocity,			DragVelocity);	//���׷����� ���� �ִ� �ӵ� ���̻����� ��ƼŬ�� �ӵ��� �ö��� �ʴ´�.
	
	SYNTHESIZE(D3DXCOLOR,	m_dStartColor,				StartColor);
	SYNTHESIZE(D3DXCOLOR,	m_dEndColor,				EndColor);
	
public:
	cParticleGroup();
	~cParticleGroup();
	
	void Setup();
	void Update();
	void Render();

private:
	void AddParticle();
	void ContinuousAdd();
	void ParticleUpdate();
};

