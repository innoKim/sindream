#pragma once
#include "iScene.h"

class cUnit;
class cMap;
class cBuilding;
class cUIObject;

class cMapEditorScene : public iScene
{
private:
	cUnit*				m_pPlayer;
	cMap*				m_pMap;
	vector<cBuilding*>	m_vecBuilding;
	cBuilding*			m_pCurrentBuilding;
	int					m_nIndexBuilding;
	vector<cUIObject*>	m_vecUIObject;
	LPD3DXSPRITE		m_pSprite;

	SYNTHESIZE(bool, m_bEditOn, EditOn);

public:
	cMapEditorScene();
	~cMapEditorScene();

	virtual void Setup() override;
	virtual void Update() override;
	virtual void Render() override;

	virtual void UIRender() override;

	static void AddOrderNexusCallback(void * CallBackObj1, void * CallBackObj2);
	static void AddOrderInhibitorCallback(void * CallBackObj1, void * CallBackObj2);
	static void AddOrderTurretCallback(void * CallBackObj1, void * CallBackObj2);
	static void AddChaosNexusCallback(void * CallBackObj1, void * CallBackObj2);
	static void AddChaosInhibitorCallback(void * CallBackObj1, void * CallBackObj2);
	static void AddChaosTurretCallback(void * CallBackObj1, void * CallBackObj2);

	static void PrevBuildingCallback(void * CallBackObj1, void * CallBackObj2);
	static void NextBuildingCallback(void * CallBackObj1, void * CallBackObj2);

	static void DeleteBuildingCallback(void * CallBackObj1, void * CallBackObj2);
	static void EnterBuildingCallback(void * CallBackObj1, void * CallBackObj2);

	static void SaveBuildingCallback(void * CallBackObj1, void * CallBackObj2);
	static void LoadBuildingCallback(void * CallBackObj1, void * CallBackObj2);

	static void AlistarSpell1CallBack(void* CallBackObj);
	static void AlistarSpell1CallBack2(void * CallBackObj);
	static void AlistarSpell2CallBack2(void * CallBackObj);
	static void AlistarSpell2CallBack(void* CallBackObj);

	virtual D3DXVECTOR3 playerPos() override;
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

