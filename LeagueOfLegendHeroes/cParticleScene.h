#pragma once
#include "iScene.h"

class cParticleGroup;

class cParticleScene :
	public iScene
{
private:
	D3DXVECTOR3 target;
	vector<cParticleGroup*> m_vecParticleGroup;

public:
	cParticleScene();
	~cParticleScene();

	virtual void Setup() override;
	virtual void Update() override;
	virtual void Render() override;
	void Pop();
};

