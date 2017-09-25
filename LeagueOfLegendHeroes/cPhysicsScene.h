#pragma once
#include "iScene.h"

class cUnit;
class cPlane;
class cMap;
class cBuilding;

class cPhysicsScene : public iScene
{
private:
	cUnit*				m_pPlayer;
	cMap*				m_pMap;
	D3DLIGHT9			m_light;
	bool				m_bEditOn;

	vector<cUnit*>		m_vecEnemy;
	vector<cBuilding*>	m_vecBuilding;
	cBuilding*			m_pCurrentBuilding;
	int					m_nIndexBuilding;
	
	void SetLight();
public:
	cPhysicsScene();
	virtual ~cPhysicsScene();

	virtual void Setup() override;
	virtual void Update() override;
	virtual void Render() override;

	static void AlistarSpell1CallBack(void* CallBackObj);
	static void AlistarSpell1CallBack2(void * CallBackObj);
	static void AlistarSpell2CallBack2(void * CallBackObj);
	static void AlistarSpell2CallBack(void* CallBackObj);

	void	SaveBuilding();
	void	LoadBuilding();

	virtual D3DXVECTOR3 playerPos() override;
};

