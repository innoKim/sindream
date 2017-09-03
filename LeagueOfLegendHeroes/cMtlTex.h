#pragma once

class cMtlTex
{
private:
	SYNTHESIZE_PASS_BY_REF(D3DMATERIAL9, m_stMtl, Mtl);
	SYNTHESIZE(LPDIRECT3DTEXTURE9, m_pTexture, Texture);

public:
	cMtlTex();
	~cMtlTex();
};

