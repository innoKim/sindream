#include "stdafx.h"
#include "cParticleScene.h"
#include "cParticleGroup.h"

cParticleScene::cParticleScene():
	target(0,0,0)
{
}

cParticleScene::~cParticleScene()
{
	for each(auto p in m_vecParticleGroup)
	{
		SAFE_DELETE(p);
	}
}

void cParticleScene::Setup()
{
	g_pShaderManager->SetTarget(&target);

	m_vecParticleGroup.resize(18);
	
	for (int i = 0; i < m_vecParticleGroup.size(); i++)
	{
		m_vecParticleGroup[i] = new cParticleGroup;

		m_vecParticleGroup[i]->SetTexturePath("texture/particleTex.tga");

		m_vecParticleGroup[i]->SetInitParticleNumber(20);
		m_vecParticleGroup[i]->SetIsContinueus(false);
		m_vecParticleGroup[i]->SetGenParticleNumber(5);

		m_vecParticleGroup[i]->SetLifeTime(1.0f);
		m_vecParticleGroup[i]->SetLifeTimeVariation(0.5f);
		
		m_vecParticleGroup[i]->SetStartPosition(D3DXVECTOR3(15 * cos(D3DX_PI/9.0f*i), 0, 15 * sin(D3DX_PI / 9.0f*i)));
		m_vecParticleGroup[i]->SetStartPositionVariation(5.0f);

		m_vecParticleGroup[i]->SetVelocity(D3DXVECTOR3(0, 0.1f, 0));
		m_vecParticleGroup[i]->SetVelocityVariation(0.1f);

		m_vecParticleGroup[i]->SetAcceleration(D3DXVECTOR3(0, 0, 0));
		m_vecParticleGroup[i]->SetAccelerationVariation(0.01f);

		m_vecParticleGroup[i]->SetStartColor(D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f));
		m_vecParticleGroup[i]->SetStartColorVariation(D3DXCOLOR(0, 0, 0, 0.3f));
		
		m_vecParticleGroup[i]->SetEndColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.0f));
		
		m_vecParticleGroup[i]->SetDragVelocity(0.05f);

		m_vecParticleGroup[i]->Setup();
	}
}

void cParticleScene::Update()
{
	for each (auto p in m_vecParticleGroup)
	{
		p->Update();
	}

	if (g_pKeyManager->IsOnceKeyDown(VK_SPACE))
	{
		Pop();
	}
}

void cParticleScene::Render()
{
	for each (auto p in m_vecParticleGroup)
	{
		p->Render();
	}
}

void cParticleScene::Pop()
{
	for each(auto p in m_vecParticleGroup)
	{
		p->Setup();
		p->Update();
	}
}
