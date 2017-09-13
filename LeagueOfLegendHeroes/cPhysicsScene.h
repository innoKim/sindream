#pragma once
#include "iScene.h"

class cUnit;

class cPhysicsScene : public iScene
{
private:
	cUnit* m_pPlayer;

public:
	cPhysicsScene();
	virtual ~cPhysicsScene();

	virtual void Setup() override;
	virtual void Update() override;
	virtual void Render() override;

	static void AlistarSpell1CallBack(void* CallBackObj);
};

