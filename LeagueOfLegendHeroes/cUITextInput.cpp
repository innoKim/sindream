#include "stdafx.h"
#include "cUITextInput.h"
#include "cUIImage.h"
#include "cUIText.h"

cUITextInput::cUITextInput()
{
}

cUITextInput::cUITextInput(string backImagePath)
	:m_szText("")
	,m_bSelected(false)
{
	m_pBack = new cUIImage;
	m_pBack->SetTexture(backImagePath);
	m_pBack->SetTag("Back");
	this->AddChild(m_pBack);

	m_pText= new cUIText;
	m_pText->SetTag("Text");
	m_pText->SetText(m_szText);
	m_pText->SetFontType(cFontManager::E_SMALL);
	m_pText->SetWidth(180);
	m_pText->SetHeight(40);
	m_pText->SetDrawTextFormat(DT_CENTER | DT_VCENTER | DT_WORDBREAK);
	this->AddChild(m_pText);
}


cUITextInput::~cUITextInput()
{
}

void cUITextInput::Update()
{
	m_pText->SetText(m_szText);
	
	if(m_bSelected)Input();
	
	cUIObject::Update();

	RECT rc;
	
	m_pBack->GetRect(&rc);

	if (PtInRect(&rc, g_ptMouse))
	{
		if(GetAsyncKeyState(VK_LBUTTON))
		{
			m_bSelected = true;
		}
	}
	else if(GetAsyncKeyState(VK_LBUTTON))
	{
		m_bSelected = false;
	}
}

void cUITextInput::Render(LPD3DXSPRITE SpritePtr)
{
	cUIObject::Render(SpritePtr);
}

void cUITextInput::Input()
{
	if ((g_cInputChar>= 33 && g_cInputChar < 127))
	{
		m_szText += g_cInputChar;
		g_cInputChar = 0;
	}
	else if(g_cInputChar == -65)
	{
		m_szText += '/';
		g_cInputChar = 0;
	}
	else if (g_cInputChar == -66)
	{
		m_szText += '.';
		g_cInputChar = 0;
	}
	else
	{
		switch ((int)g_cInputChar)
		{
		case 8: //backspace
			if(m_szText.size()>0)m_szText.pop_back();
			g_cInputChar = 0;
			break;
		}
	}
}
