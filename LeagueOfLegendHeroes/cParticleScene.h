#pragma once
#include "iScene.h"

class cParticleGroup;
class cUIObject;
class cUnit;
class cPlayer;
class cMap;

class cParticleScene : public iScene
{
private:
	enum  eType
	{
		eNone,
		eFloat,
		eInt,
		eVector3,
		eEnd
	};

public:
	eType m_eNone;
	eType m_eFloat;
	eType m_eInt;
	eType m_eVector3;

	D3DXVECTOR3 target;
	cUnit* m_pPlayer;

	cParticleGroup* m_pCurParticleGroup;

	//for UI
	LPD3DXSPRITE m_pSprite;
	vector<cUIObject*>		m_vecUIObject;

	cMap* m_pMap;

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

	virtual D3DXVECTOR3 playerPos() override;
};

