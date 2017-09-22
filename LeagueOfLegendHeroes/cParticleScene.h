#pragma once
#include "iScene.h"

class cParticleGroup;
class cUIObject;
class cUnit;
class cPlayer;

class cParticleScene : public iScene
{
private:
	
	D3DXVECTOR3 target;
	cUnit* m_pPlayer;

	cParticleGroup* m_pParticleGroup;

	//for UI
	LPD3DXSPRITE m_pSprite;
	vector<cUIObject*>		m_vecUIObject;


public:
	cParticleScene();
	~cParticleScene();

	virtual void Setup() override;
	virtual void Update() override;
	virtual void Render() override;
	void Pop();

	void RenderInfo();

	static void AlistarSpell1CallBack(void* CallBackObj);
	static void AlistarSpell2CallBack(void* CallBackObj);
};

