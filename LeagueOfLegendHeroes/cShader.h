#pragma once


class cShader
{
private:
	LPD3DXMESH				m_pMesh;
	LPD3DXMESH				m_pDisc;
	LPD3DXEFFECT			m_pApplyShadow;
	LPD3DXEFFECT			m_pCreateShadow;
	D3DXVECTOR3*			m_pvCameraPos;
	D3DXVECTOR4				m_vLightPos;
	LPDIRECT3DTEXTURE9		m_pDMTexture;
	LPDIRECT3DTEXTURE9		m_pSMTexture;
	LPDIRECT3DTEXTURE9		m_pNMTexture;
	LPDIRECT3DCUBETEXTURE9	m_pEnvTexture;
	LPDIRECT3DTEXTURE9		m_pShadowRenderTarget;
	LPDIRECT3DSURFACE9		m_pShadowDepthStencil;
	D3DXVECTOR4				m_vLightColor;
	D3DXVECTOR4				m_vTorusColor;
	D3DXVECTOR4				m_vDiscColor;
	string					m_sFolder;
	float					m_fRotationY;

public:
	cShader();
	~cShader();

	void SetupShadow(D3DXVECTOR3* pvEye, char* szApplyFileName, char* szCreateFileName, char* szMeshFileName);
	void Setup(D3DXVECTOR3* pvEye, char* szFxFileName, char* szMeshFileName, char* szDMTextureFileName = NULL, char* szSMTextureFileName = NULL, char* szNMTextureFileName = NULL, char* szEnvTextureFileName = NULL);
	void Render();

private:
	LPD3DXEFFECT LoadEffect(char* szFileName);
};

