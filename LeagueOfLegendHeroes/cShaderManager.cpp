#include "stdafx.h"
#include "cShaderManager.h"
#include "cGroup.h"
#include "cMtlTex.h"

cShaderManager::cShaderManager()
	: m_pApplyShadow(NULL)
	, m_pCreateShadow(NULL)
	, m_pShadowRenderTarget(NULL)
	, m_pShadowDepthStencil(NULL)
	, m_vLightColor(D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f))
	, m_vLightPos(-1, 3500, -1, 1.0f)
	, m_pHWBackBuffer(NULL)
	, m_pHWDepthStencilBuffer(NULL)
	, m_pMeshGround(NULL)
	, m_pCubeTexture(NULL)
	, m_pSkybox(NULL)
	, m_pCube(NULL)
	, m_bSkyboxOn(true)
{
}


cShaderManager::~cShaderManager()
{
}

void cShaderManager::Destroy()
{
	SAFE_RELEASE(m_pShadowRenderTarget);
	SAFE_RELEASE(m_pApplyShadow);
	SAFE_RELEASE(m_pCreateShadow);
	SAFE_RELEASE(m_pShadowDepthStencil);
	SAFE_RELEASE(m_pCubeTexture);
	SAFE_RELEASE(m_pSkybox);
	SAFE_RELEASE(m_pCube);
}

void cShaderManager::SetupShadow()
{
	m_pCreateShadow = LoadEffect("shader/CreateShadow.fx");

	m_pApplyShadow = LoadEffect("shader/ApplyShadow.fx");

	
	//광원-투영 행렬을 만든다
	D3DXMatrixPerspectiveFovLH(&m_matLightProjection, D3DX_PI / 4.0f, 1, 1, 3600);
	m_pCreateShadow->SetMatrix("matLightProjection", &m_matLightProjection);

	if (m_bSkyboxOn)
	{
		m_pSkybox = LoadEffect("shader/Skybox.fx");
		D3DXCreateCubeTextureFromFile(g_pD3DDevice, "shader/cubeTexture1.dds", &m_pCubeTexture);
		D3DXCreateBox(g_pD3DDevice, 20000, 20000, 20000, &m_pCube, NULL);
	}

	//쉐도우 맵 가로, 세로 사이즈
	const int shadowMapSize = 8192;

	//렌더타겟 변수에 쉐도우 맵 생성
	g_pD3DDevice->CreateTexture(shadowMapSize, shadowMapSize, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &m_pShadowRenderTarget, NULL);

	//쉐도우 맵에서 사용될 깊이 버퍼 생성
	g_pD3DDevice->CreateDepthStencilSurface(shadowMapSize, shadowMapSize, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pShadowDepthStencil, NULL);

	//알파테스트 설정, 알파값이 0인 경우 그리지 않는다
	g_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 0);
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

	m_pApplyShadow->SetFloat("fLightWeight", 1.0f);
	m_pApplyShadow->SetBool("bTexture", true);
}

void cShaderManager::RenderShadow(LPD3DXMESH pMesh, LPDIRECT3DTEXTURE9 pTexture, D3DXMATRIXA16 matWorld)
{
	//2패스 렌더링을 위해 벡터에 메쉬와 텍스처, 월드 매트릭스들을 저장
	m_vecMesh.push_back(pMesh);
	m_vecTexture.push_back(pTexture);
	m_vecMatWorld.push_back(matWorld);

	//광원-뷰 행렬을 만든다
	D3DXVECTOR3 vEyePt, vLookatPt;
	if (m_pvTarget)
	{
		vEyePt = D3DXVECTOR3(m_pvTarget->x + m_vLightPos.x, m_pvTarget->y + m_vLightPos.y, m_pvTarget->z + m_vLightPos.z);
		vLookatPt = *m_pvTarget;
	}
	else
	{
		vEyePt = D3DXVECTOR3(m_vLightPos.x, m_vLightPos.y, m_vLightPos.z);
		vLookatPt = D3DXVECTOR3(0, 0, 0);
	}

	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);

	D3DXMatrixLookAtLH(&m_matLightView, &vEyePt, &vLookatPt, &vUpVec);


	m_pCreateShadow->SetMatrix("matWorld", &matWorld);
	m_pCreateShadow->SetMatrix("matLightView", &m_matLightView);

	//현재 렌더 타겟은 쉐도우 맵, 쉐도우 맵에 광원 위치에서 본 오브젝트의 모양을 그린다
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
	//렌더 타겟을 원래의 하드웨어 백버퍼와 깊이 버퍼로 교체
	g_pD3DDevice->SetRenderTarget(0, m_pHWBackBuffer);
	g_pD3DDevice->SetDepthStencilSurface(m_pHWDepthStencilBuffer);

	//더 이상 사용하지 않는 임시 변수 메모리 해제
	SAFE_RELEASE(m_pHWBackBuffer);
	SAFE_RELEASE(m_pHWDepthStencilBuffer);

	D3DXMATRIXA16 matView, matProjection;

	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProjection);

	//뷰-투영 매트릭스
	m_matViewProjection = matView * matProjection;

	D3DXVECTOR4 vCameraPos = D3DXVECTOR4(g_pCamera->GetPos(), 1.0f);
	

	UINT numPasses;
	if (m_bSkyboxOn)
	{
		m_pSkybox->SetMatrix("matViewProjection", &m_matViewProjection);
		m_pSkybox->SetVector("vCameraPos", &vCameraPos);
		m_pSkybox->SetTexture("EnvironmentMap_Tex", m_pCubeTexture);

		numPasses = 0;
		m_pSkybox->Begin(&numPasses, NULL);

		for (int i = 0; i < numPasses; i++)
		{
			m_pSkybox->BeginPass(i);

			m_pCube->DrawSubset(0);

			m_pSkybox->EndPass();
		}
	
		m_pSkybox->End();
	}
	
	//2패스 렌더링에 필요한 각종 변수 설정
	m_pApplyShadow->SetMatrix("matLightView", &m_matLightView);
	m_pApplyShadow->SetMatrix("matLightProjection", &m_matLightProjection);
	m_pApplyShadow->SetMatrix("matViewProjection", &m_matViewProjection);

	D3DXVECTOR4 vLightPos;
	if (m_pvTarget)
	{
		vLightPos = D3DXVECTOR4(m_pvTarget->x + m_vLightPos.x, m_pvTarget->y + m_vLightPos.y, m_pvTarget->z + m_vLightPos.z, 1);
	}
	else
	{
		vLightPos = m_vLightPos;
	}
	m_pApplyShadow->SetVector("vLightPos", &vLightPos);
	m_pApplyShadow->SetVector("vCameraPos", &D3DXVECTOR4(g_pCamera->GetPos(), 1.0f));

	//그림자 그리는데 필요한 쉐도우 맵 렌더타겟 설정
	m_pApplyShadow->SetTexture("ShadowMap_Tex", m_pShadowRenderTarget);

	numPasses = 0;
	m_pApplyShadow->Begin(&numPasses, NULL);

	for (int i = 0; i < numPasses; i++)
	{
		m_pApplyShadow->BeginPass(i);
		
		//1패스때 저장한 메쉬들 각각의 월드 매트릭스와 텍스처를 통하여 그림자가 입혀진 상태로 렌더링
		for (int j = 0; j < m_vecMesh.size(); j++)
		{
			m_pApplyShadow->SetMatrix("matWorld", &m_vecMatWorld[j]);
			m_pApplyShadow->SetTexture("DiffuseMap_Tex", m_vecTexture[j]);
			m_pApplyShadow->CommitChanges();

			m_vecMesh[j]->DrawSubset(0);
		}

		//각 오브젝트들의 그림자가 입혀진 바닥 맵 렌더링
		if (m_pMeshGround)
		{
			m_pApplyShadow->SetMatrix("matWorld", &m_matWorldGround);

			for (int k = 0; k < m_vecMtlTex.size(); k++)
			{
				m_pApplyShadow->SetFloat("fLightWeight", 2.0f);
				m_pApplyShadow->SetTexture("DiffuseMap_Tex", m_vecMtlTex[k]->GetTexture());
				m_pApplyShadow->CommitChanges();

				m_pMeshGround->DrawSubset(k);
			}
		}

		m_pApplyShadow->EndPass();
	}

	m_pApplyShadow->End();

	m_vecMesh.clear();
	m_vecTexture.clear();
	m_vecMatWorld.clear();
}

void cShaderManager::SetPlane(LPD3DXMESH pMesh, D3DXMATRIXA16 matWorld)
{
	m_pMeshGround = pMesh;

	m_matWorldGround = matWorld;
}

void cShaderManager::SetMap(LPD3DXMESH pMesh, vector<cMtlTex*> vecMtlTex, D3DXMATRIXA16 matWorldGround)
{
	m_pMeshGround = pMesh;
	m_matWorldGround = matWorldGround;
	m_vecMtlTex = vecMtlTex;
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
