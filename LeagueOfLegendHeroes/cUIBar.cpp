#include "stdafx.h"
#include "cUIBar.h"
#include "cUIImage.h"
#include "cUnit.h"

cUIBar::cUIBar():
	m_pUnit(NULL),
	m_pBackBar(NULL),
	m_pFrontBar(NULL),
	m_fOffsetX(0.0f),
	m_fOffsetY(0.0f)
{
}

cUIBar::cUIBar(cUnit* TargetUnit, string BackPath, string FrontPath, float OffsetX, float OffsetY)
{
	m_pUnit = TargetUnit;

	m_pBackBar = new cUIImage;
	m_pBackBar->SetTexture(BackPath);
	m_pBackBar->SetTag("BackBar");
	this->AddChild(m_pBackBar);
	
	m_pFrontBar = new cUIImage;
	m_pFrontBar->SetTexture(FrontPath);
	m_pFrontBar->SetTag("FrontBar");
	m_pBackBar->AddChild(m_pFrontBar);
	
	m_fOffsetX = OffsetX;
	m_fOffsetY = OffsetY;
}

cUIBar::~cUIBar()
{
}

void cUIBar::Update()
{
	cUIObject::Update();
}

void cUIBar::Render(LPD3DXSPRITE SpritePtr)
{
	if (m_pUnit)
	{
		D3DXVECTOR3 screenPos = MY_UTIL::WorldToScreenPosition((m_pUnit->GetPosition()));
		m_pBackBar->SetPosition(screenPos.x-80, screenPos.y -150);
		m_pFrontBar->SetPosition(m_fOffsetX, m_fOffsetY);
		cUIObject::Render(SpritePtr);
	}
}
