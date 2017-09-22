#pragma once
#include "cUIObject.h"

class cUIImage;
class cUnit;

class cUIBar :	public cUIObject
{
protected:
	cUIImage* m_pBackBar;
	cUIImage* m_pFrontBar;

	SYNTHESIZE(cUnit*, m_pUnit, TargetUnit);

	SYNTHESIZE(float, m_fOffsetX, OffsetX);
	SYNTHESIZE(float, m_fOffsetY, OffsetY);

	cUIBar();
public:
	cUIBar(cUnit* TargetUnit, string BackPath, string FrontPath, float OffsetX, float OffsetY);
	virtual ~cUIBar();

	virtual void Update();
	virtual void Render(LPD3DXSPRITE SpritePtr);
};
