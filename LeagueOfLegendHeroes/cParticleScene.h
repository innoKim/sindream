#pragma once
#include "iScene.h"

class cParticleGroup;
class cUIObject;
class cUnit;
class cPlayer;
class cMap;
class cGridPlane;

class cParticleScene : public iScene
{
private:
	enum  eType
	{
		eNone,
		eFloat,
		eInt,
		eVector3,
		eColor,
		eEnd
	};

public:
	eType m_eNone;
	eType m_eFloat;
	eType m_eInt;
	eType m_eVector3;
	eType m_eColor;


	D3DXVECTOR3 target;
	cUnit* m_pPlayer;

	cParticleGroup* m_pCurParticleGroup;
	vector<cParticleGroup*> m_vecCurParticleGroup;

	//for UI
	LPD3DXSPRITE m_pSprite;
	vector<cUIObject*>		m_vecUIObject;

	cMap* m_pMap;
	cGridPlane* m_pGrid;

	SYNTHESIZE(void*, m_pCurValue,CurValuePtr);
	SYNTHESIZE(eType, m_eValueType, ValueType);


public:
	cParticleScene();
	~cParticleScene();

	virtual void Setup() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void UIRender() override;

	void Pop();
	void RenderInfo();

	static void AlistarSpell1CallBack(void* CallBackObj);
	static void AlistarSpell2CallBack(void* CallBackObj);
	static void SetVariableToChange(void * scene, void * variable, void *variabletype);
	static void SaveButton(void* CallBackObj);
	static void LoadButton(void* CallBackObj);
	virtual D3DXVECTOR3 playerPos() override;

private:

	void UISetup();
	void TextBoxPrint();
	void ValueControl();
	void Save();
	void Load();
};

