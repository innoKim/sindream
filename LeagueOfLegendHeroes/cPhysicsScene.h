#pragma once
#include "iScene.h"

class cPhysicsScene : public iScene
{


public:
	cPhysicsScene();
	virtual ~cPhysicsScene();

	virtual void Setup() override;
	virtual void Update() override;
	virtual void Render() override;
};

