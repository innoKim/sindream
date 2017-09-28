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
	void LoadEffects(string szFilePath); 

public:	
	void PlayEffect(string szFilePath, D3DXVECTOR3 position);
	void Update();
	void Render();
	
	void Destroy();
	void ClearWork();

	cParticleGroup* NewEffect();

	//void CloneGroupVector(IN vector<cParticleGroup*> in, OUT vector<cParticleGroup*> out);
};

