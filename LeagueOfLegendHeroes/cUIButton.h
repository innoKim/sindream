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
	
	SYNTHESIZE(CALLBACKFUNC, m_pfnCallback, Callback);
	SYNTHESIZE(void*, m_pCallbackObj, CallbackObject);
	SYNTHESIZE(void*, m_pCallbackObj2, CallbackObject2);


public:
	cUIButton();
	virtual ~cUIButton();

	virtual void Update() override;
	virtual void Render(LPD3DXSPRITE SpritePtr) override;

	virtual void SetTexture(string Normal, string MouseOver, string Selected);
};

