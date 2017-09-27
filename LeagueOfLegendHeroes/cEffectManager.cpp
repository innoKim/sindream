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

void cEffectManager::LoadEffects()
{

}

cParticleGroup * cEffectManager::NewEffect()
{
	cParticleGroup* newParticleEffect = new cParticleGroup;
	newParticleEffect->Setup();

	return newParticleEffect;
}



void cEffectManager::AddToStorage(string effectKey, vector<cParticleGroup*> particleGroup)
{
	map<string, vector<cParticleGroup*>>::iterator itor;
	itor = m_mapStorage.find(effectKey);
	
	if (itor == m_mapStorage.end())
	{
		m_mapStorage.insert(pair<string, vector<cParticleGroup*>>(effectKey, particleGroup));
	}
	else
	{
		for each(auto p in itor->second)
		{
			SAFE_DELETE(p);
		}

		itor->second = particleGroup;
	}
}

void cEffectManager::DeleteFromStorage(string effectKey)
{
	map<string, vector<cParticleGroup*>>::iterator itor;
	itor = m_mapStorage.find(effectKey);

	if (itor == m_mapStorage.end())
	{
		for each(auto p in itor->second)
		{
			SAFE_DELETE(p);
		}
		m_mapStorage.erase(itor);
	}
}

void cEffectManager::PlayEffect(string effectKey)
{
	map<string, vector<cParticleGroup*>>::iterator itor;
	itor = m_mapStorage.find(effectKey);

	if (itor == m_mapStorage.end()) return;


	vector<cParticleGroup*> newVector;

	for each (auto p in itor->second)
	{
		cParticleGroup* clone = new cParticleGroup;

		clone->SetTexturePath(p->GetTexturePath());

		clone->SetIsContinueus(p->GetIsContinueus());

		clone->SetInitParticleNumber(p->GetInitParticleNumber());
		clone->SetGenParticleNumber(p->GetGenParticleNumber());

		clone->SetLifeTime(p->GetLifeTime());
		clone->SetLifeTimeVariation(p->GetLifeTimeVariation());

		clone->SetStartPosition(p->GetStartPosition());
		clone->SetStartPositionVariation(p->GetStartPositionVariation());

		clone->SetVelocity(p->GetVelocity());
		clone->SetVelocityVariation(p->GetVelocityVariation());
		clone->SetDragVelocity(p->GetDragVelocity());

		clone->SetAcceleration(p->GetAcceleration());
		clone->SetAccelerationVariation(p->GetAccelerationVariation());

		clone->SetStartColor(p->GetStartColor());
		clone->SetStartColorVariation(p->GetStartColorVariation());

		clone->SetEndColor(p->GetEndColor());
		clone->SetEndColorVariation(p->GetEndColorVariation());

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
