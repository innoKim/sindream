#pragma once


class cShader
{
private:
	LPD3DXMESH			m_pMesh;
	LPD3DXEFFECT		m_pEffect;
	D3DXVECTOR4			m_vCameraPos;
	D3DXVECTOR4			m_vLightPos;
	LPDIRECT3DTEXTURE9	m_pDMTexture;
	LPDIRECT3DTEXTURE9	m_pSMTexture;
	D3DXVECTOR4			m_vLightColor;

public:
	cShader();
	~cShader();

	void Setup(char* szFxFileName, char* szMeshFileName, char* szDMTextureFileName = NULL, char* szSMTextureFileName = NULL);
	void Render();

private:
	LPD3DXEFFECT LoadEffect(char* szFileName);
};

