#pragma once
#include "cUIObject.h"
class cUIScroll : public cUIObject
{
public:
	cUIScroll();
	virtual ~cUIScroll();
};
//
//D3DXCreateSprite(g_pD3DDevice, &m_pSprite);
//
//m_pFrustum = new cFrustum;
//
//cUIImageView* pImageView = new cUIImageView;
//pImageView->SetTexture("UI/panel-info.png");
//m_pUIRoot = pImageView;
//
//cUITextView* pTextView = new cUITextView;
//pTextView->SetText("동대문 역사문화 공원에서 한대원 10마리를 잡아오세요.");
//pTextView->SetSize(ST_SIZE(312, 200));
//pTextView->SetPosition(100, 100);
//pTextView->SetDrawTextFormat(DT_CENTER | DT_VCENTER | DT_WORDBREAK);
//pTextView->SetTag(E_TEXT_VIEW);
//m_pUIRoot->AddChild(pTextView);
//
//cUIButton* pButton = new cUIButton;
//pButton->SetTexture("UI/btn-med-up.png",
//	"UI/btn-med-over.png",
//	"UI/btn-med-down.png");
//pButton->SetPosition(135, 330);
//pButton->SetDelegate(this);
//pButton->SetTag(E_CONFIRM_BUTTON);
//m_pUIRoot->AddChild(pButton);
//
//pButton = new cUIButton;
//pButton->SetTexture("UI/btn-med-up.png",
//	"UI/btn-med-over.png",
//	"UI/btn-med-down.png");
//pButton->SetPosition(135, 400);
//pButton->SetDelegate(this);
//pButton->SetTag(E_CANCEL_BUTTON);
//m_pUIRoot->AddChild(pButton);