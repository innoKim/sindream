#include "stdafx.h"
#include "cUIImage.h"


cUIImage::cUIImage()
{
}


cUIImage::~cUIImage()
{
}

void cUIImage::SetTexture(string sFullPath)
{
	m_sTexture = sFullPath;
}

void cUIImage::Render(LPD3DXSPRITE SpritePtr)
{
	D3DXIMAGE_INFO stImageInfo;
	LPDIRECT3DTEXTURE9 pTexture = g_pTextureManager->GetTextureEx(m_sTexture, &stImageInfo);
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

void cUIImage::GetRect(RECT * RectPtr)
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);

	D3DXIMAGE_INFO stImageInfo;
	LPDIRECT3DTEXTURE9 pTexture = g_pTextureManager->GetTextureEx(m_sTexture, &stImageInfo);

	float widthRatio, heightRatio;
	widthRatio = (float)rc.right / (float)WIN_WIDTH;
	heightRatio = (float)rc.bottom / (float)WIN_HEIGHT;

	if (!RectPtr) return;

	RectPtr->left = m_matWorld._41*widthRatio;
	RectPtr->top = m_matWorld._42*heightRatio;
	RectPtr->right = (m_matWorld._41 + stImageInfo.Width)*widthRatio;
	RectPtr->bottom = (m_matWorld._42 + stImageInfo.Height)*heightRatio;
}
