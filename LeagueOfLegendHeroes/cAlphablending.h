#pragma once
class cAlphablending
{
private:
	vector<ST_PT_VERTEX>	m_vecVertex;
	bool					m_bBillboarding;
	bool					m_bBlending;
	bool					m_bEnd;
	float					m_fLifeTime;
	float					m_fCurrentTime;
	int						m_nSize;
	LPDIRECT3DTEXTURE9		m_pTexture;
	SYNTHESIZE(D3DXVECTOR3, m_vPos, Positition);
	SYNTHESIZE(D3DXVECTOR3, m_vDir, Direction);
	SYNTHESIZE(string,		m_sTag, Tag);

public:
	cAlphablending();
	~cAlphablending();

	void Setup(string sTag, string sTextureFileName, D3DXVECTOR3 vPos, D3DXVECTOR3 vDir, float fLifeTime, int nSize, bool bBlending = false, bool bBillboarding = false);
	void Update();
	void UIRender();
	bool IsEnd() { return m_bEnd; }
	void Play() { m_bEnd = false; m_fCurrentTime = 0.0f; }
};
