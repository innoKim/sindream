#include "stdafx.h"
#include "cParticleGroup.h"

cParticleGroup::cParticleGroup():
	m_szTexturePath(""),
	m_bIsContinuous(false),
	m_nInitParticle(1),
	m_nGenParticle(0),
	m_fLifeTime(1.0f),
	m_fLifeTimeVariation(0.0f),
	m_vStartPosition(0,0,0),
	m_vStartPositionVariation(0.0f),
	m_vVelocity(0, 0, 0),
	m_vVelocityVariation(0.0f),
	m_fDragVelocity(0.0f),
	m_dStartColor(1,1,1,1),
	m_dEndColor(0,0,0,0)
{
}

cParticleGroup::~cParticleGroup()
{

}

void cParticleGroup::Setup()
{
	while (m_vecVertex.size() < m_nInitParticle)
	{
		AddParticle();
	}
}

void cParticleGroup::Update()
{
	if (m_bIsContinuous)
	{
		ContinuousAdd();
	}

	ParticleUpdate();
}

void cParticleGroup::Render()
{
}

void cParticleGroup::AddParticle()
{
}

void cParticleGroup::ContinuousAdd()
{
}

void cParticleGroup::ParticleUpdate()
{
}
