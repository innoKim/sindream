#include "stdafx.h"
#include "cEffectManager.h"
#include "cParticleGroup.h"

cEffectManager::cEffectManager()
{

}

cEffectManager::~cEffectManager()
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

	m_vecWork.push_back(itor->second);
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

void cEffectManager::ClearWork()
{
	for each(auto p in m_vecWork)
	{
		SAFE_DELETE(p);
	}
	m_vecWork.clear();
}
