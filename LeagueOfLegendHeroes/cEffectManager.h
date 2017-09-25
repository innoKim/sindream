#pragma once

#define g_pEffectManager cEffectManager::GetInstance()

class cParticleGroup;

class cEffectManager
{
	SINGLETON(cEffectManager);

private:
	map<string, cParticleGroup* >			m_mapStorage;
	
	vector<cParticleGroup*>					m_vecWork;
	
public:
	void LoadEffects();
	cParticleGroup* NewEffect();
	void AddToStorage(string effectKey, cParticleGroup* particleGroup);
	void DeleteFromStorage(string effectKey);
	void SaveEffects();
	void PlayEffect(string effectKey);
	void Update();
	void Render();

	void ClearWork();
};

