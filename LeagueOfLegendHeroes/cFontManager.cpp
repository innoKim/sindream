#include "stdafx.h"
#include "cFontManager.h"


cFontManager::cFontManager()
{
}


cFontManager::~cFontManager()
{
}


LPD3DXFONT cFontManager::GetFont(eFontType e)
{
	if (m_mapFont.find(e) == m_mapFont.end())
	{
		D3DXFONT_DESC fd;
		ZeroMemory(&fd, sizeof(D3DXFONT_DESC));

		if (e == E_NORMAL)
		{
			fd.Height = 24;
			fd.Width = 12;
			fd.Weight = FW_NORMAL;
			fd.Italic = false;
			fd.CharSet = DEFAULT_CHARSET;
			fd.OutputPrecision = OUT_DEFAULT_PRECIS;
			fd.PitchAndFamily = FF_DONTCARE;
			//AddFontResource("umberto.ttf");
			strcpy_s(fd.FaceName, "���� ����");
		}

		else if (e == E_SMALL)
		{
			fd.Height = 12;
			fd.Width = 6;
			fd.Weight = FW_NORMAL;
			fd.Italic = false;
			fd.CharSet = DEFAULT_CHARSET;
			fd.OutputPrecision = OUT_DEFAULT_PRECIS;
			fd.PitchAndFamily = FF_DONTCARE;
			//AddFontResource("umberto.ttf");
			strcpy_s(fd.FaceName, "���� ����");
		}
		
		D3DXCreateFontIndirect(g_pD3DDevice, &fd, &m_mapFont[e]);
	}

	return m_mapFont[e];
}

void cFontManager::Destroy()
{
	for each(auto it in m_mapFont)
	{
		SAFE_RELEASE(it.second);
	}
}
