#include "stdafx.h"
#include "cDebugManager.h"


cDebugManager::cDebugManager()
{
	Setup();
}


cDebugManager::~cDebugManager()
{
}

void cDebugManager::Setup()
{
	D3DXFONT_DESC DXFont_DESC;
	ZeroMemory(&DXFont_DESC, sizeof(D3DXFONT_DESC));

	DXFont_DESC.Height = 15;
	DXFont_DESC.Width = 15;
	DXFont_DESC.Weight = FW_NORMAL;
	DXFont_DESC.MipLevels = D3DX_DEFAULT;
	DXFont_DESC.Italic = false;
	DXFont_DESC.CharSet = DEFAULT_CHARSET;
	DXFont_DESC.OutputPrecision = OUT_DEFAULT_PRECIS;
	DXFont_DESC.Quality = DEFAULT_QUALITY;
	DXFont_DESC.PitchAndFamily = DEFAULT_PITCH;
	DXFont_DESC.FaceName, TEXT("¸¼Àº °íµñ");
	D3DXCreateFontIndirect(g_pD3DDevice, &DXFont_DESC, &m_pFont);
}

void cDebugManager::Destroy()
{
	if (m_pFont)m_pFont->Release();
}

void cDebugManager::Print(char * str, int nX, int nY, D3DXCOLOR color)
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	RECT rt = { nX, nY, rc.right, rc.bottom };
	if (m_pFont)m_pFont->DrawText(0, str, strlen(str), &rt, DT_TOP | DT_LEFT, color);
}
