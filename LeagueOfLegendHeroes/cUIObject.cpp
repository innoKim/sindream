#include "stdafx.h"
#include "cUIObject.h"


cUIObject::cUIObject()
	: m_vRelativePosition(0, 0, 0)
	, m_fWidth(0.0f)
	, m_fHeight(0.0f)
	, m_pParent(NULL)
	, m_szTag("")
{
	D3DXMatrixIdentity(&m_matWorld);
}


cUIObject::~cUIObject()
{
	for each(auto p in m_vecChild)
	{
		SAFE_DELETE(p);
	}
}

void cUIObject::Update()
{
	m_matWorld._41 = m_vRelativePosition.x;
	m_matWorld._42 = m_vRelativePosition.y;
	if (m_pParent)
	{
		m_matWorld._41 += m_pParent->m_matWorld._41;
		m_matWorld._42 += m_pParent->m_matWorld._42;
	}

	for each(auto p in m_vecChild)
	{
		p->Update();
	}
}

void cUIObject::Render(LPD3DXSPRITE SpritePtr)
{
	for each(auto p in m_vecChild)
	{
		p->Render(SpritePtr);
	}
}

void cUIObject::SetPosition(float x, float y)
{
	m_vRelativePosition.x = x;
	m_vRelativePosition.y = y;
	m_vRelativePosition.z = 0;
}

void cUIObject::GetRect(RECT * RectPtr)
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);

	float widthRatio, heightRatio;
	widthRatio = (float)rc.right / (float)WIN_WIDTH;
	heightRatio = (float)rc.bottom / (float)WIN_HEIGHT;

	if (!RectPtr) return;
	
	RectPtr->left = m_matWorld._41*widthRatio;
	RectPtr->top = m_matWorld._42*heightRatio;
	RectPtr->right = (m_matWorld._41 + m_fWidth)*widthRatio;
	RectPtr->bottom = (m_matWorld._42 + m_fHeight)*heightRatio;
}

void cUIObject::AddChild(cUIObject * ChildPtr)
{
	ChildPtr->m_pParent = this;
	m_vecChild.push_back(ChildPtr);
}

cUIObject * cUIObject::GetChild(string Tag)
{
	if (m_szTag == Tag)
	{
		return this;
	}

	cUIObject* pChild = NULL;
	for each(auto p in m_vecChild)
	{
		if (p->GetTag() == Tag)
		{
			pChild = p;
			break;
		}
	}
	return pChild;
}
