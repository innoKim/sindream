#pragma once
#include "iScene.h"

class cUnit;
class cMap;
class cBuilding;
class cUIButton;

class cMapEditorScene : public iScene
{
private:
	cUnit*				m_pPlayer;
	cMap*				m_pMap;
	bool				m_bEditOn;
	vector<cBuilding*>	m_vecBuilding;
	cBuilding*			m_pCurrentBuilding;
	int					m_nIndexBuilding;
	vector<cUIButton*>	m_vecButton;

public:
	cMapEditorScene();
	~cMapEditorScene();

	virtual void Setup() override;
	virtual void Update() override;
	virtual void Render() override;

	static void AlistarSpell1CallBack(void* CallBackObj);
	static void AlistarSpell1CallBack2(void * CallBackObj);
	static void AlistarSpell2CallBack2(void * CallBackObj);
	static void AlistarSpell2CallBack(void* CallBackObj);

	virtual D3DXVECTOR3 playerPos() override;
};

