#pragma once
#include "iScene.h"

class cUnit;

class cAnimationTestScene :
	public iScene
{
private:
	cUnit* m_pTestUnit;

public:
	cAnimationTestScene();
	virtual ~cAnimationTestScene();

	virtual void Setup() override;
	virtual void Update() override;
	virtual void Render() override;

	virtual void UIRender() override;
};

