#pragma once

#define RAY 9999.f

#include "iScene.h"
class cMap;
class cUnit;
class cBuilding;

class cTestScene : public iScene
{
private:
	cUnit*					m_pPlayer;
	cMap*					m_pMap;
	vector<cBuilding*>		m_vecBuilding;
	bool					m_bEditOn;

public:
	cTestScene();
	virtual ~cTestScene();

	virtual void Setup() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void UIRender() override;

	static void AlistarSpell1CallBack(void* CallBackObj);
	static void AlistarSpell2CallBack(void* CallBackObj);

	void SetLight();

};

