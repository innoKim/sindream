#include "stdafx.h"
#include "cEffectManager.h"
#include "cParticleGroup.h"
#include "cEffectLoader.h"

cEffectManager::cEffectManager()
{

}

cEffectManager::~cEffectManager()
{	
}

bool cEffectManager::IsEffectDead(vector<cParticleGroup*>& particleEffect)
{
	for each(auto p in particleEffect)
	{
		if (!p->IsDead())
		{
			return false;
		}
	}
	return true;
}

void cEffectManager::LoadEffects(string szFilePath)
{
	map<string, vector<cParticleGroup*>>::iterator mapItor;
	mapItor = m_mapStorage.find(szFilePath);

	if (mapItor == m_mapStorage.end())
	{
		cEffectLoader el;

		vector<cParticleGroup*> vecGroup;
		el.LoadEffect(szFilePath, vecGroup);

		m_mapStorage.insert(pair < string, vector<cParticleGroup*>>(szFilePath, vecGroup));
	}
}

cParticleGroup * cEffectManager::NewEffect()
{
	cParticleGroup* newParticleEffect = new cParticleGroup;
	newParticleEffect->Setup();

	return newParticleEffect;
}

void cEffectManager::PlayEffect(string szFilePath, D3DXVECTOR3 position)
{
	map<string, vector<cParticleGroup*>>::iterator itor;
	itor = m_mapStorage.find(szFilePath);

	if (itor == m_mapStorage.end())
	{
		LoadEffects(szFilePath);
		itor = m_mapStorage.find(szFilePath);

		if (itor == m_mapStorage.end()) return;
	}
	
	vector<cParticleGroup*> newVector;

	for each (auto p in itor->second)
	{
		cParticleGroup* clone = new cParticleGroup;
		clone->Clone(p);
		clone->SetStartPosition(clone->GetStartPosition() + position);
		clone->Setup();
		newVector.push_back(clone);
	}

	m_vecWork.push_back(newVector);
}

void cEffectManager::Update()
{
	vector<vector<cParticleGroup*>>::iterator itor = m_vecWork.begin();
	
	for (itor; itor != m_vecWork.end();)
	{
		if (IsEffectDead((*itor)))
		{
			for each(auto p in *itor)
			{
				SAFE_DELETE(p);
			}
			itor = m_vecWork.erase(itor);
		}
		else
		{
			for each(auto p in (*itor))
			{
				p->Update();
			}
			itor++;
		}
	}
}

void cEffectManager::Render()
{
	for each (auto p in m_vecWork)
	{
		for each (auto q in p)
		{
			q->Render();
		}
	}
}

void cEffectManager::Destroy()
{
	for each(auto p in m_mapStorage)
	{
		for each (auto q in p.second)
		{
			SAFE_DELETE(q);
		}		
	}

	for each(auto p in m_vecWork)
	{
		for each (auto q in p)
		{
			SAFE_DELETE(q);
		}
	}
}

void cEffectManager::ClearWork()
{
	for each(auto p in m_vecWork)
	{
		for each (auto q in p)
		{
			SAFE_DELETE(q);
		}
	}
	m_vecWork.clear();
}
