#pragma once

#define RAY 9999.f

#include "iScene.h"
#include "cMap.h"
#include "cPlayer.h"

class cTestScene : public iScene
{
private:
	cUnit*					m_pPlayer;
	cMap*					m_pMap;

public:
	cTestScene();
	virtual ~cTestScene();

	virtual void Setup() override;
	virtual void Update() override;
	virtual void Render() override;

	static void AlistarSpell1CallBack(void* CallBackObj);
	static void AlistarSpell2CallBack(void* CallBackObj);

	void SetLight();
};

