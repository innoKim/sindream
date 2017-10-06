#pragma once
#include "iScene.h"

class cParticleGroup;
class cUIObject;
class cUnit;
class cPlayer;
class cMap;
class cGridPlane;
class cUITextInput;

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

	//for UI
	LPD3DXSPRITE m_pSprite;
	vector<cUIObject*>		m_vecUIObject;
	
	cGridPlane* m_pGrid;

	SYNTHESIZE(cParticleGroup*, m_pCurParticleGroup, CurParticleGroupPtr);
	SYNTHESIZE(cUITextInput*, m_pTextInput, TextInputPtr);
	SYNTHESIZE(void*, m_pCurValue,CurValuePtr);
	SYNTHESIZE(eType, m_eValueType, ValueType);

	SYNTHESIZE_PASS_BY_REF(vector<cParticleGroup*>, m_vecCurParticleGroup, CurParticleGroupVectorRef);

public:
	cParticleScene();
	~cParticleScene();

	virtual void Setup() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void UIRender() override;

	void Pop();
	void PopVector();
	void RenderInfo();
	void ClearParticleVector();

//콜백 함수들
	static void AlistarSpell1CallBack(void* CallBackObj);
	static void AlistarSpell2CallBack(void* CallBackObj);
	static void SetVariableToChange(void * scene, void * variable, void *variabletype);
	static void SaveButtonCallBack(void* CallBackObj);
	static void LoadButtonCallBack(void* CallBackObj);
	static void SetTextureCallBack(void* CallBackObj);
	static void NewParticleCallBack(void* CallBackObj);
	static void InitCallBack(void* CallBackObj);

private:

	void UISetup();
	void TextBoxPrint();
	void ValueControl();
	void Save();
	void Load();
	void SetParticlePos();
};

