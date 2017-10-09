#pragma once
#include "iScene.h"

#define THROW_SPD 1000


class cUnit;
class cEnemy;
class cMap;
class cBuilding;
class cPlane;

class cPhysicsScene : public iScene
{
private:
	cUnit*				m_pPlayer;
	cMap*				m_pMap;
	cPlane*				m_pGrid;
	D3DLIGHT9			m_light;
	bool				m_bEditOn;

	vector<cEnemy*>		m_vecEnemy;
	vector<cBuilding*>	m_vecBuilding;
	cBuilding*			m_pCurrentBuilding;
	int					m_nIndexBuilding;
	
	int				m_nElasticFactor;

	void SetLight();
public:
	cPhysicsScene();
	virtual ~cPhysicsScene();

	virtual void Setup() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void UIRender() override;

	static void AlistarSpell1CallBack(void* CallBackObj);
	static void AlistarSpell1CallBack2(void * CallBackObj);
	static void AlistarSpell2CallBack2(void * CallBackObj);
	static void AlistarSpell2CallBack(void* CallBackObj);
};

