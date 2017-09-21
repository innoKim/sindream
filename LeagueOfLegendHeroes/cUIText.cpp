#include "stdafx.h"
#include "cUIText.h"


cUIText::cUIText()
	: m_eFontType(cFontManager::E_NORMAL)
	, m_dwDT(DT_LEFT | DT_TOP | DT_NOCLIP)
	, m_dwColor(D3DCOLOR_XRGB(255, 255, 255))
{
}


cUIText::~cUIText()
{
}

void cUIText::Render(LPD3DXSPRITE SpritePtr)
{
	RECT rc;
	rc.left = m_matWorld._41;
	rc.top = m_matWorld._42;
	rc.right = m_matWorld._41 + m_fWidth;
	rc.bottom = m_matWorld._42 + m_fHeight;

	LPD3DXFONT pFont = g_pFontManager->GetFont(m_eFontType);
	pFont->DrawTextA(NULL,
		m_szText.c_str(),
		m_szText.length(),
		&rc,
		m_dwDT,
		m_dwColor);
}
