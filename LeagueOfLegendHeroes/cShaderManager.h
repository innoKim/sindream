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
	D3DXVECTOR4					m_vLightDirection;			//광원의 Direction
	LPDIRECT3DTEXTURE9			m_pShadowRenderTarget;		//1패스때 쉐도우맵 그리는데 사용할 렌더타겟
	LPDIRECT3DSURFACE9			m_pShadowDepthStencil;		//쉐도우맵 그리는데 사용할 깊이 버퍼
	D3DXVECTOR4					m_vLightColor;				//광원의 색
	LPDIRECT3DSURFACE9			m_pHWBackBuffer;			//원래 사용중이던 백버퍼를 임시 저장할 변수
	LPDIRECT3DSURFACE9			m_pHWDepthStencilBuffer;	//원래 사용중이던 깊이 버퍼를 임시 저장할 변수
	vector<LPD3DXMESH>			m_vecMesh;					//그림자를 적용할 오브젝트들을 저장할 벡터
	vector<LPDIRECT3DTEXTURE9>	m_vecTexture;				//각 오브젝트에 적용될 텍스처
	vector<D3DXMATRIXA16>		m_vecMatWorld;				//각 오브젝트에 적용될 월드 매트릭스
	D3DXMATRIXA16				m_matLightView;				//광원-뷰 매트릭스
	D3DXMATRIXA16				m_matLightProjection;		//광원-투영 매트릭스
	D3DXMATRIXA16				m_matViewProjection;		//뷰-투영 매트릭스
	D3DXVECTOR3*				m_pvTarget;					//카메라가 따라다니는 타겟
	LPD3DXMESH					m_pMeshGround;				//맵으로 사용될 메쉬
	D3DXMATRIXA16				m_matWorldGround;			//맵에 적용될 월드 매트릭스
	vector<cMtlTex*>			m_vecMtlTex;				//맵에 적용될 텍스처
	LPDIRECT3DCUBETEXTURE9		m_pCubeTexture;				//스카이박스로 사용될 큐브 텍스처
	LPD3DXEFFECT				m_pSkybox;					//스카이박스를 그리기 위한 쉐이더
	LPD3DXMESH					m_pCube;

public:
	void BeginRender();		//그림자를 그리기 위하여 렌더할 때마다 초기화를 위해 호출되는 begin함수
	void SetupShadow();		//쉐도우 맵을 위한 셋업 함수
	void RenderShadow(LPD3DXMESH pMesh, LPDIRECT3DTEXTURE9 pTexture, D3DXMATRIXA16 matWorld);	//쉐도우 맵에 광원 위치에서 보이는 오브젝트의 거리(깊이값)들을 저장할 1패스 렌더링
	void Render();	//그림자가 입혀진 오브젝트들을 실질적으로 그려주는 렌더 함수
	void Destroy();
	void SetPlane(LPD3DXMESH pMesh, D3DXMATRIXA16 matWorld);	//텍스처 없이 단일 메쉬로 이루어진 맵 셋업
	void SetMap(LPD3DXMESH pMesh, vector<cMtlTex*> vecMtlTex, D3DXMATRIXA16 matWorldGround);	//단일 메쉬이되 여러 개의 텍스처로 이루어진 맵 셋업
	void SetTarget(D3DXVECTOR3* pvTarget) { m_pvTarget = pvTarget; }	//타겟 설정

private:
	LPD3DXEFFECT LoadEffect(char* szFileName);
};

