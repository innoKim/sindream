#pragma once

class cParticle;

class cParticleGroup
{
private:
	vector<ST_PC_VERTEX>	m_vecVertex;
	vector<cParticle*>		m_vecParticle;
	
	SYNTHESIZE_PASS_BY_REF(string,		m_szTexturePath,			TexturePath);

	//SYNTHESIZE_PASS_BY_REF(bool,		m_bIsContinuous,			IsContinueus);
	
	SYNTHESIZE_PASS_BY_REF(int,			m_nInitParticle,			InitParticleNumber);
	
	SYNTHESIZE_PASS_BY_REF(float,		m_fLifeTime,				LifeTime);
	SYNTHESIZE_PASS_BY_REF(float,		m_fLifeTimeVariation,		LifeTimeVariation);
	
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vStartPosition,			StartPosition);
	SYNTHESIZE_PASS_BY_REF(float,		m_fStartPositionVariation,	StartPositionVariation);
	
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vVelocity,				Velocity);
	SYNTHESIZE_PASS_BY_REF(float,		m_fVelocityVariation,		VelocityVariation);
	SYNTHESIZE_PASS_BY_REF(float,		m_fDragVelocity,			DragVelocity);	//저항력으로 인한 최대 속도 이이상으로 파티클의 속도가 올라가지 않는다.
	
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vAcceleration,			Acceleration);
	SYNTHESIZE_PASS_BY_REF(float,		m_fAccelerationVariation,	AccelerationVariation);
	
	SYNTHESIZE_PASS_BY_REF(D3DXCOLOR,	m_dStartColor,				StartColor);
	SYNTHESIZE_PASS_BY_REF(D3DXCOLOR,	m_cStartColorVariation,		StartColorVariation);
	SYNTHESIZE_PASS_BY_REF(D3DXCOLOR,	m_dEndColor,				EndColor);
	SYNTHESIZE_PASS_BY_REF(D3DXCOLOR,	m_cEndColorVariation,		EndColorVariation);
	
public:
	cParticleGroup();
	~cParticleGroup();
	
	void Setup();
	void Update();
	void Render();

	bool IsDead() { return (m_vecParticle.size() == 0); }

	void Init();
	void Clone(cParticleGroup* target);
private:

	void AddParticle();
	void ParticleUpdate();
};

