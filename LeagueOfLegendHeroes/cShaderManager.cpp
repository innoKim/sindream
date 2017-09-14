#include "stdafx.h"
#include "cShaderManager.h"


cShaderManager::cShaderManager()
	: m_pApplyShadow(NULL)
	, m_pCreateShadow(NULL)
	, m_pShadowRenderTarget(NULL)
	, m_vLightColor(D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f))
	, m_vLightPos(D3DXVECTOR4(-500.0f, 500.0f, -500.0f, 1.0f))
	, m_sFolder("shader/")
	, m_pHWBackBuffer(NULL)
	, m_pHWDepthStencilBuffer(NULL)
	, m_pMeshGround(NULL)
{
}


cShaderManager::~cShaderManager()
{
}

void cShaderManager::BeginRender()
{
	//현재 하드웨어 백버퍼와 깊이버퍼
	g_pD3DDevice->GetRenderTarget(0, &m_pHWBackBuffer);
	g_pD3DDevice->GetDepthStencilSurface(&m_pHWDepthStencilBuffer);

	// 1. 그림자 만들기
	LPDIRECT3DSURFACE9 pShadowSurface = NULL;

	if (SUCCEEDED(m_pShadowRenderTarget->GetSurfaceLevel(0, &pShadowSurface)))
	{
		g_pD3DDevice->SetRenderTarget(0, pShadowSurface);
		SAFE_RELEASE(pShadowSurface);
	}

	g_pD3DDevice->SetDepthStencilSurface(m_pShadowDepthStencil);

	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);

	//광원-뷰 행렬을 만든다
	D3DXVECTOR3 vEyePt(m_pvTarget->x + m_vLightPos.x, m_pvTarget->y + m_vLightPos.y, m_pvTarget->z + m_vLightPos.z);

	D3DXVECTOR3 vLookatPt(0, 0, 0);

	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);

	D3DXMatrixLookAtLH(&m_matLightView, &vEyePt, &vLookatPt, &vUpVec);

	//광원-투영 행렬을 만든다
	D3DXMatrixPerspectiveFovLH(&m_matLightProjection, D3DX_PI / 4.0f, 1, 1, 10000);

	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);

	m_pCreateShadow->SetMatrix("matWorld", &matWorld);
	m_pCreateShadow->SetMatrix("matLightView", &m_matLightView);
	m_pCreateShadow->SetMatrix("matLightProjection", &m_matLightProjection);
	m_pCreateShadow->SetVector("vLightPos", &D3DXVECTOR4(vEyePt, 1));

	m_pApplyShadow->SetBool("bTexture", true);
}

void cShaderManager::SetupShadow()
{
	m_pCreateShadow = LoadEffect("shader/CreateShadow.fx");

	m_pApplyShadow = LoadEffect("shader/ApplyShadow.fx");

	const int shadowMapSize = 8192;

	g_pD3DDevice->CreateTexture(shadowMapSize, shadowMapSize, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &m_pShadowRenderTarget, NULL);

	g_pD3DDevice->CreateDepthStencilSurface(shadowMapSize, shadowMapSize, D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pShadowDepthStencil, NULL);
}

void cShaderManager::RenderShadow(LPD3DXMESH pMesh, LPDIRECT3DTEXTURE9 pTexture, D3DXMATRIXA16 matWorld)
{
	m_vecMesh.push_back(pMesh);
	m_vecTexture.push_back(pTexture);
	m_vecMatWorld.push_back(matWorld);

	m_pCreateShadow->SetMatrix("matWorld", &matWorld);

	UINT numPasses = 0;
	m_pCreateShadow->Begin(&numPasses, NULL);

	for (int i = 0; i < numPasses; i++)
	{
		m_pCreateShadow->BeginPass(i);

		pMesh->DrawSubset(0);

		m_pCreateShadow->EndPass();
	}

	m_pCreateShadow->End();
}

void cShaderManager::Render()
{
	//2. 그림자 입히기
	g_pD3DDevice->SetRenderTarget(0, m_pHWBackBuffer);
	g_pD3DDevice->SetDepthStencilSurface(m_pHWDepthStencilBuffer);

	SAFE_RELEASE(m_pHWBackBuffer);
	SAFE_RELEASE(m_pHWDepthStencilBuffer);

	D3DXMATRIXA16 matView, matProjection; 

	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProjection);

	m_matViewProjection = matView * matProjection;

	m_pApplyShadow->SetMatrix("matLightView", &m_matLightView);
	m_pApplyShadow->SetMatrix("matLightProjection", &m_matLightProjection);
	m_pApplyShadow->SetMatrix("matViewProjection", &m_matViewProjection);

	m_pApplyShadow->SetVector("vLightPos", &D3DXVECTOR4(m_pvTarget->x + m_vLightPos.x, m_pvTarget->y + m_vLightPos.y, m_pvTarget->z + m_vLightPos.z, 1));
	m_pApplyShadow->SetVector("vCameraPos", &D3DXVECTOR4(g_pCamera->GetPos(), 1.0f));

	m_pApplyShadow->SetTexture("ShadowMap_Tex", m_pShadowRenderTarget);

	UINT numPasses = 0;
	m_pApplyShadow->Begin(&numPasses, NULL);

	for (int i = 0; i < numPasses; i++)
	{
		m_pApplyShadow->BeginPass(i);
		
		for (int j = 0; j < m_vecMesh.size(); j++)
		{
			m_pApplyShadow->SetMatrix("matWorld", &m_vecMatWorld[j]);
			m_pApplyShadow->CommitChanges();

			m_pApplyShadow->SetTexture("DiffuseMap_Tex", m_vecTexture[j]);
			m_pApplyShadow->CommitChanges();

			m_vecMesh[j]->DrawSubset(0);
		}

		if (m_pMeshGround)
		{
			m_pApplyShadow->SetMatrix("matWorld", &m_matWorldGround);
			m_pApplyShadow->SetBool("bTexture", false);

			m_pApplyShadow->CommitChanges();

			m_pMeshGround->DrawSubset(0);
		}

		m_pApplyShadow->EndPass();
	}

	m_pvTarget;

	m_pApplyShadow->End();

	m_vecMesh.clear();
	m_vecTexture.clear();
	m_vecMatWorld.clear();

}

void cShaderManager::Destroy()
{
	SAFE_RELEASE(m_pShadowRenderTarget);
	SAFE_RELEASE(m_pApplyShadow);
	SAFE_RELEASE(m_pCreateShadow);
	SAFE_RELEASE(m_pShadowDepthStencil);
}

void cShaderManager::SetPlane(LPD3DXMESH pMesh, D3DXMATRIXA16 matWorld)
{
	m_pMeshGround = pMesh;

	m_matWorldGround = matWorld;
}

LPD3DXEFFECT cShaderManager::LoadEffect(char * szFileName)
{
	LPD3DXEFFECT pEffect = NULL;

	// 셰이더 로딩
	LPD3DXBUFFER		pError = NULL;			//에러 버퍼 ( 셰이더를 컴파일할때 잘못 된 문법이나 오류정보를 리턴해주는 버퍼 )
	DWORD				dwShaderFlag = 0;		//셰이더 플레그 0 

#ifdef _DEBUG
	dwShaderFlag = dwShaderFlag | D3DXSHADER_DEBUG;		//셰이더를 디버그모드로 컴파일하겠다 ( 디버그모드로 해야 잘못된 컴파일 오류가 날때 Error 버퍼에 오류정보가 들어간다 ) 
#endif

														//fx 파일로 부터 셰이더 객체 생성
	D3DXCreateEffectFromFile(
		g_pD3DDevice,				// 디바이스
		szFileName,					// 불러올 셰이더 코드 파일이름
		NULL,						// 셰이더를 컴파일할때 추가로 사용할 #define 정의 ( 일단 NULL )
		NULL,						// 셰이더를 컴파일할때 #include 지시문을 처리할때 사용할 인터페이스 플레그 ( 일단 NULL )
		dwShaderFlag,				// 셰이더 컴파일 플레그
		NULL,						// 셰이더 매개변수를 공유할 메모리풀 ( 일단 NULL )
		&pEffect,					// 로딩될 셰이더 Effect 포인터
		&pError						// 셰이더를 로딩하고 컴파일할때 문제가 생기면 해당 버퍼에 에러메시지가 들어간다 ( 성공적으로 로딩되면 NULL 이 참조된다 )
	);

	//셰이더 파일 로딩에 문제가 있다면..
	if (pError != NULL || pEffect == NULL) {

		//문제의 내용이 뭔지 문자열로 확인
		int size = pError->GetBufferSize();
		char* str = new char[size];

		//str에 버퍼에있는 내용을 저장한다.
		sprintf_s(str, size, (const char*)pError->GetBufferPointer());

		OutputDebugString(str);
		//오류내용을 출력했으니 오류버퍼 해제
		SAFE_RELEASE(pError);
		SAFE_DELETE_ARRAY(str);

		return NULL;
	}

	return pEffect;
}
