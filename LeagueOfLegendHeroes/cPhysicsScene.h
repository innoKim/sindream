#pragma once
#include "iScene.h"

class cUnit;
class cPlane;
class cMap;

class cPhysicsScene : public iScene
{
private:
	cUnit* m_pPlayer;
	cMap* m_pMap;
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
	static void AlistarSpell1CallBack2(void * CallBackObj);
	static void AlistarSpell2CallBack(void* CallBackObj);

	virtual D3DXVECTOR3 playerPos() override;
};

