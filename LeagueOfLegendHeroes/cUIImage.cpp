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
