#pragma once
class cUIObject
{
protected:

	SYNTHESIZE(string,	m_szTag,	Tag);     //태그임다 스트링으로 바로 찾아쓸수있습니다.

	SYNTHESIZE(float,	m_fWidth,	Width);
	SYNTHESIZE(float,	m_fHeight,	Height);

	D3DXVECTOR3			m_vRelativePosition;
	D3DXMATRIX			m_matWorld;

	vector<cUIObject*>	m_vecChild; 
	cUIObject*			m_pParent;

public:
	cUIObject();
	virtual ~cUIObject();

	virtual void Update();
	virtual void Render(LPD3DXSPRITE SpritePtr);

	virtual void SetPosition(float x, float y);
	virtual void GetRect(RECT* RectPtr);

	virtual void AddChild(cUIObject* ChildPtr);
	virtual cUIObject* GetChild(string Tag);
};

