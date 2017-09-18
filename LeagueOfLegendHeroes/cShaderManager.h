#pragma once

#define g_pShaderManager cShaderManager::GetInstance()

class cGroup;
class cMtlTex;

class cShaderManager
{
private:
	SINGLETON(cShaderManager);

private:
	LPD3DXEFFECT				m_pApplyShadow;				//2패스때 사용할 쉐이더
	LPD3DXEFFECT				m_pCreateShadow;			//1패스때 사용할 쉐이더
	D3DXVECTOR4					m_vLightPos;				//캐릭터를 기준으로 한 광원의 상대적 위치
	LPDIRECT3DTEXTURE9			m_pShadowRenderTarget;		//1패스때 쉐도우맵 그리는데 사용할 렌더타겟
	LPDIRECT3DSURFACE9			m_pShadowDepthStencil;		//
	D3DXVECTOR4					m_vLightColor;
	string						m_sFolder;
	LPDIRECT3DSURFACE9			m_pHWBackBuffer;
	LPDIRECT3DSURFACE9			m_pHWDepthStencilBuffer;
	LPD3DXMESH					m_pMeshGround;
	D3DXMATRIXA16				m_matWorldGround;
	vector<LPD3DXMESH>			m_vecMesh;
	vector<LPDIRECT3DTEXTURE9>	m_vecTexture;
	vector<D3DXMATRIXA16>		m_vecMatWorld;
	D3DXMATRIXA16				m_matLightView;
	D3DXMATRIXA16				m_matLightProjection;
	D3DXMATRIXA16				m_matViewProjection;
	D3DXVECTOR3*				m_pvTarget;
	vector<cMtlTex*>			m_vecMtlTex;

public:
	void BeginRender();
	void SetupShadow();
	void RenderShadow(LPD3DXMESH pMesh, LPDIRECT3DTEXTURE9 pTexture, D3DXMATRIXA16 matWorld);
	void Render();
	void Destroy();
	void SetPlane(LPD3DXMESH pMesh, D3DXMATRIXA16 matWorld);
	void SetMap(LPD3DXMESH pMesh, vector<cMtlTex*> vecMtlTex, D3DXMATRIXA16 matWorldGround);
	void SetTarget(D3DXVECTOR3* pvTarget) { m_pvTarget = pvTarget; }

private:
	LPD3DXEFFECT LoadEffect(char* szFileName);
};

