#pragma once
class cEffectLoader
{
public:
	cEffectLoader();
	virtual ~cEffectLoader();

	void LoadEffect(IN string filePath, OUT vector<cParticleGroup*>& out);
	void SaveEffect(IN string filePath, IN vector<cParticleGroup*>& in);
};

