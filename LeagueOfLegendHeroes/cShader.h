#pragma once


class cShader
{
private:
	LPD3DXMESH		m_pMesh;
	LPD3DXEFFECT	m_pEffect;
	D3DXVECTOR4		m_vCameraPos;
	D3DXVECTOR4		m_vLightPos;

public:
	cShader();
	~cShader();

	void Setup(char* szFxFileName, char* szMeshFileName);
	void Render();

private:
	LPD3DXEFFECT LoadEffect(char* szFileName);
};

