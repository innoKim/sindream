#pragma once
#include "cUIObject.h"
class cUIText : public cUIObject
{
protected:
	SYNTHESIZE(string, m_szText, Text);
	SYNTHESIZE(cFontManager::eFontType, m_eFontType, FontType);
	SYNTHESIZE(DWORD, m_dwDT, DrawTextFormat);
	SYNTHESIZE(D3DCOLOR, m_dwColor, Color);

public:
	cUIText();
	virtual ~cUIText();

	virtual void Render(LPD3DXSPRITE SpritePtr) override;
};

