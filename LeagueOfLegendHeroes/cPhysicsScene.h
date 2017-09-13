#pragma once
#include "iScene.h"

class cUnit;
class cPlane;

class cPhysicsScene : public iScene
{
private:
	cUnit* m_pPlayer;
	cPlane* m_pPlane;
	D3DLIGHT9	m_light;

	vector<cUnit*> m_vecEnemy;

	void SetLight();
public:
	cPhysicsScene();
	virtual ~cPhysicsScene();

	virtual void Setup() override;
	virtual void Update() override;
	virtual void Render() override;

	static void AlistarSpell1CallBack(void* CallBackObj);
	static void AlistarSpell2CallBack(void* CallBackObj);
};

