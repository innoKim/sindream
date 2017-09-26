#pragma once
class cAlphablending
{
private:
	bool					m_bBillboarding;
	float					m_fTime;
	LPDIRECT3DTEXTURE9		m_pTexture;
	SYNTHESIZE(D3DXVECTOR3, m_vPos, Positition);
	SYNTHESIZE(D3DXVECTOR3, m_vDirection, Direction);

public:
	cAlphablending();
	~cAlphablending();

	void Setup();
	void UIRender();
};
