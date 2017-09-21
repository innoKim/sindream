#pragma once

#define RAY 9999.f

#include "iScene.h"
#include "cMap.h"

class cUnit;

class cTestScene : public iScene
{
private:
	cUnit*					m_pPlayer;
	cMap*					m_pMap;
	vector<cUnit*>			m_vecBuilding;

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

