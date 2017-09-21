#pragma once

#define g_pFontManager cFontManager::GetInstance()

class cFontManager
{
	SINGLETON(cFontManager);

public:
	enum eFontType
	{
		E_NORMAL,
		E_END,
	};

private:
	map<eFontType, LPD3DXFONT> m_mapFont;

public:
	LPD3DXFONT GetFont(eFontType e);
	void Destroy();
};

