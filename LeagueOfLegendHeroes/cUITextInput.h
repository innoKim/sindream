#pragma once

#include "cUIObject.h"
#include <stdio.h>

class cUIImage;
class cUIText;

class cUITextInput : public cUIObject
{
private:
	cUIImage*	m_pBack;
	cUIText*	m_pText;
	SYNTHESIZE_PASS_BY_REF(string, m_szText, Text);
	SYNTHESIZE_PASS_BY_REF(bool,	m_bSelected,IsSelected);

public:
	cUITextInput();
	cUITextInput(string backImagePath);
	virtual ~cUITextInput();

	virtual void Update() override;
	virtual void Render(LPD3DXSPRITE SpritePtr) override;

private:
	void Input();
};

