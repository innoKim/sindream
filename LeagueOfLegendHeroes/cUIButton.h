#pragma once
#include "cUIObject.h"


class cUIButton : public cUIObject
{
protected:
	enum eButtonStatus
	{
		E_NORMAL,
		E_MOUSEOVER,
		E_SELECTED,
		E_STATUS_COUNT,
	};
	
	eButtonStatus	m_eButtonStatus;
	string			m_arrTexture[E_STATUS_COUNT];
	
	SYNTHESIZE(CALLBACKFUNC,	m_pfnCallback1,		Callback);
	SYNTHESIZE(CALLBACKFUNC2,	m_pfnCallback2,		Callback2);
	SYNTHESIZE(CALLBACKFUNC3,	m_pfnCallback3,		Callback3);
	SYNTHESIZE(void*,			m_pCallbackObj1,	CallbackObject);
	SYNTHESIZE(void*,			m_pCallbackObj2,	CallbackObject2);
	SYNTHESIZE(void*,			m_pCallbackObj3,	CallbackObject3);


public:
	cUIButton();
	virtual ~cUIButton();

	virtual void Update() override;
	virtual void Render(LPD3DXSPRITE SpritePtr) override;

	virtual void SetTexture(string Normal, string MouseOver, string Selected);
};

