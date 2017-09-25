#include "stdafx.h"
#include "cEffectManager.h"
#include "cParticleGroup.h"

cEffectManager::cEffectManager()
{

}

cEffectManager::~cEffectManager()
{
	
}

void cEffectManager::SaveEffects()
{

}

void cEffectManager::LoadEffects()
{

}

cParticleGroup * cEffectManager::NewEffect()
{
	cParticleGroup* newParticleEffect = new cParticleGroup;
	newParticleEffect->Setup();

	return newParticleEffect;
}



void cEffectManager::AddToStorage(string effectKey, cParticleGroup* particleGroup)
{
	map<string, cParticleGroup*>::iterator itor;
	itor = m_mapStorage.find(effectKey);
	
	if (itor == m_mapStorage.end())
	{
		m_mapStorage.insert(pair<string, cParticleGroup*>(effectKey, particleGroup));
	}
	else
	{
		SAFE_DELETE(itor->second);
		itor->second = particleGroup;
	}
}

void cEffectManager::DeleteFromStorage(string effectKey)
{
	map<string, cParticleGroup*>::iterator itor;
	itor = m_mapStorage.find(effectKey);

	if (itor == m_mapStorage.end())
	{
		SAFE_DELETE(itor->second);
		m_mapStorage.erase(itor);
	}
}

void cEffectManager::PlayEffect(string effectKey)
{
	map<string, cParticleGroup*>::iterator itor;
	itor = m_mapStorage.find(effectKey);

	if (itor == m_mapStorage.end()) return;

	cParticleGroup* clone = new cParticleGroup;

	clone->SetTexturePath			(itor->second->GetTexturePath());

	clone->SetIsContinueus			(itor->second->GetIsContinueus());
	
	clone->SetInitParticleNumber	(itor->second->GetInitParticleNumber());
	clone->SetGenParticleNumber		(itor->second->GetGenParticleNumber());
	
	clone->SetLifeTime				(itor->second->GetLifeTime());
	clone->SetLifeTimeVariation		(itor->second->GetLifeTimeVariation());
	
	clone->SetStartPosition			(itor->second->GetStartPosition());
	clone->SetStartPositionVariation(itor->second->GetStartPositionVariation());
	
	clone->SetVelocity				(itor->second->GetVelocity());
	clone->SetVelocityVariation		(itor->second->GetVelocityVariation());
	clone->SetDragVelocity			(itor->second->GetDragVelocity());
	
	clone->SetAcceleration			(itor->second->GetAcceleration());
	clone->SetAccelerationVariation	(itor->second->GetAccelerationVariation());
	
	clone->SetStartColor			(itor->second->GetStartColor());
	clone->SetStartColorVariation	(itor->second->GetStartColorVariation());
	
	clone->SetEndColor				(itor->second->GetEndColor());
	clone->SetEndColorVariation		(itor->second->GetEndColorVariation());

	clone->Setup();
	
	m_vecWork.push_back(clone);
}

void cEffectManager::Update()
{
	vector<cParticleGroup*>::iterator itor = m_vecWork.begin();
	
	for (itor; itor != m_vecWork.end();)
	{
		if ((*itor)->IsDead())
		{
			SAFE_DELETE(*itor);
			itor = m_vecWork.erase(itor);
		}
		else
		{
			(*itor)->Update();
			itor++;
		}
	}
}

void cEffectManager::Render()
{
	for each (auto p in m_vecWork)
	{
		p->Render();
	}
}

void cEffectManager::Destroy()
{
	for each(auto p in m_mapStorage)
	{
		SAFE_DELETE(p.second);
	}

	for each(auto p in m_vecWork)
	{
		SAFE_DELETE(p);
	}
}

void cEffectManager::ClearWork()
{
	for each(auto p in m_vecWork)
	{
		SAFE_DELETE(p);
	}
	m_vecWork.clear();
}
