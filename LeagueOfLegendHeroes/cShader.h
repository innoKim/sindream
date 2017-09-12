#pragma once


class cShader
{
private:
	LPD3DXMESH				m_pMesh;
	LPD3DXEFFECT			m_pEffect;
	D3DXVECTOR3*			m_pvCameraPos;
	D3DXVECTOR4				m_vLightPos;
	LPDIRECT3DTEXTURE9		m_pDMTexture;
	LPDIRECT3DTEXTURE9		m_pSMTexture;
	LPDIRECT3DTEXTURE9		m_pNMTexture;
	LPDIRECT3DCUBETEXTURE9	m_pEnvTexture;
	D3DXVECTOR4				m_vLightColor;
	string					m_sFolder;
	float					m_fRotationY;

public:
	cShader();
	~cShader();

	void Setup(D3DXVECTOR3* pvEye, char* szFxFileName, char* szMeshFileName, char* szDMTextureFileName = NULL, char* szSMTextureFileName = NULL, char* szNMTextureFileName = NULL, char* szEnvTextureFileName = NULL);
	void Render();

private:
	LPD3DXEFFECT LoadEffect(char* szFileName);
};

