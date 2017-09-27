#pragma once

#define g_pEffectManager cEffectManager::GetInstance()

class cParticleGroup;

class cEffectManager
{
	SINGLETON(cEffectManager);

private:
	map<string, vector<cParticleGroup*> >	m_mapStorage;
	
	vector<vector<cParticleGroup*>>			m_vecWork;

	bool IsEffectDead(vector<cParticleGroup*>& particleEffect);
	
public:
	void LoadEffects();
	cParticleGroup* NewEffect();
	void AddToStorage(string effectKey, vector<cParticleGroup*> particleGroup);
	void DeleteFromStorage(string effectKey);
	void SaveEffects();
	void PlayEffect(string effectKey);
	void Update();
	void Render();
	
	void Destroy();
	void ClearWork();
};

