#pragma once
#include "iScene.h"
class cParticleScene :
	public iScene
{
public:
	cParticleScene();
	~cParticleScene();

	virtual void Setup() override;
	virtual void Update() override;
	virtual void Render() override;
};

