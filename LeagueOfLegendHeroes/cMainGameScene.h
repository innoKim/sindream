#pragma once
#include "iScene.h"

class cUnit;
class cMap;
class cBuilding;
class cUIObject;
class cAlphablending;
class cEnemy;

class cMainGameScene : public iScene
{
private:
	cUnit*					m_pPlayer;
	cMap*					m_pMap;
	cBuilding*				m_pCurrentBuilding;
	int						m_nIndexBuilding;
	LPD3DXSPRITE			m_pSprite;
	vector<cBuilding*>		m_vecBuilding;
	vector<cUIObject*>		m_vecUIObject;
	vector<cEnemy*>			m_vecEnemy;

	SYNTHESIZE(bool, m_bEditOn, EditOn);

public:
	cMainGameScene();
	~cMainGameScene();

	virtual void Setup() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void UIRender() override;

	static void AddOrderNexusCallback(void * CallBackObj);
	static void AddOrderInhibitorCallback(void * CallBackObj);
	static void AddOrderTurretCallback(void * CallBackObj);
	static void AddChaosNexusCallback(void * CallBackObj);
	static void AddChaosInhibitorCallback(void * CallBackObj);
	static void AddChaosTurretCallback(void * CallBackObj);

	static void PrevBuildingCallback(void * CallBackObj);
	static void NextBuildingCallback(void * CallBackObj);

	static void DeleteBuildingCallback(void * CallBackObj);
	static void EnterBuildingCallback(void * CallBackObj);

	static void SaveBuildingCallback(void * CallBackObj);
	static void LoadBuildingCallback(void * CallBackObj);

	static void AlistarWalkCallBack(void* CallBackObj);
	static void AlistarSpell1CallBack(void* CallBackObj);
	static void AlistarSpell1CallBack2(void * CallBackObj);
	static void AlistarSpell2CallBack2(void * CallBackObj);
	static void AlistarSpell2CallBack(void* CallBackObj);
private:
	bool AddOrderNexus(void);
	bool AddOrderInhibitor(void);
	bool AddOrderTurret(void);
	bool AddChaosNexus(void);
	bool AddChaosInhibitor(void);
	bool AddChaosTurret(void);

	bool PrevBuilding();
	bool NextBuilding();

	bool DeleteBuilding();
	bool EnterBuilding();

	bool SaveBuilding(char* szFileName);
	bool LoadBuilding(char* szFileName);
};

