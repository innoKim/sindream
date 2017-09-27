#pragma once
#include "cUIObject.h"
class cUIImage : public cUIObject
{
protected:
	string m_sTexture;

public:
	cUIImage();
	virtual ~cUIImage();

	virtual void SetTexture(string sFullPath);
	virtual void Render(LPD3DXSPRITE SpritePtr) override;

	virtual void GetRect(RECT * RectPtr) override;
};

