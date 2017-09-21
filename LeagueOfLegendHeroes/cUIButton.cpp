#include "stdafx.h"
#include "cUIButton.h"


cUIButton::cUIButton()
	: m_eButtonStatus(E_NORMAL)
	, m_pfnCallback(NULL)
	, m_pCallbackObj(NULL)
{
}


cUIButton::~cUIButton()
{
}

void cUIButton::Update()
{
	RECT rc;
	GetRect(&rc);

	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if (PtInRect(&rc, ptMouse))
	{
		if (GetKeyState(VK_LBUTTON) & 0x8000)
		{
			if (m_eButtonStatus == E_MOUSEOVER)
			{
				m_eButtonStatus = E_SELECTED;
			}
		}
		else
		{
			if (m_eButtonStatus == E_SELECTED)
			{
				if (m_pfnCallback)
					m_pfnCallback(m_pCallbackObj);
			}
			m_eButtonStatus = E_MOUSEOVER;
		}
	}
	else
	{
		m_eButtonStatus = E_NORMAL;
	}

	cUIObject::Update();
}

void cUIButton::Render(LPD3DXSPRITE SpritePtr)
{
	D3DXIMAGE_INFO stImageInfo;
	LPDIRECT3DTEXTURE9 pTexture = g_pTextureManager->GetTextureEx(m_arrTexture[m_eButtonStatus], &stImageInfo);
	SpritePtr->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	SpritePtr->SetTransform(&m_matWorld);
	RECT rc;
	SetRect(&rc, 0, 0, stImageInfo.Width, stImageInfo.Height);
	SpritePtr->Draw(pTexture,
		&rc,
		&D3DXVECTOR3(0, 0, 0),
		&D3DXVECTOR3(0, 0, 0),
		D3DCOLOR_ARGB(255, 255, 255, 255));

	SpritePtr->End();

	cUIObject::Render(SpritePtr);
}

void cUIButton::SetTexture(string Normal, string MouseOver, string Selected)
{
	m_arrTexture[E_NORMAL] = Normal;
	m_arrTexture[E_MOUSEOVER] = MouseOver;
	m_arrTexture[E_SELECTED] = Selected;

	D3DXIMAGE_INFO stImageInfo;
	g_pTextureManager->GetTextureEx(Normal, &stImageInfo);
	m_fWidth = stImageInfo.Width;
	m_fHeight = stImageInfo.Height;
}
