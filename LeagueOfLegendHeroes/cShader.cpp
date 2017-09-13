#include "stdafx.h"
#include "cShader.h"


cShader::cShader()
	: m_pApplyShadow(NULL)
	, m_pCreateShadow(NULL)
	, m_pMesh(NULL)
	, m_pDisc(NULL)
	, m_pDMTexture(NULL)
	, m_pSMTexture(NULL)
	, m_pNMTexture(NULL)
	, m_pEnvTexture(NULL)
	, m_pvCameraPos(NULL)
	, m_pShadowRenderTarget(NULL)
	, m_vLightColor(D3DXVECTOR4(0.7f, 0.7f, 1.0f, 1.0f))
	, m_vTorusColor(D3DXVECTOR4(1, 1, 0, 1))
	, m_vDiscColor(D3DXVECTOR4(0, 1, 1, 1))
	, m_vLightPos(D3DXVECTOR4(-300.0f, 300.0f, -300.0f, 1.0f))
	, m_sFolder("shader/")
	, m_fRotationY(0.0f)
{
}


cShader::~cShader()
{
	m_pvCameraPos = NULL;

	SAFE_RELEASE(m_pShadowRenderTarget);
	SAFE_RELEASE(m_pEnvTexture);
	SAFE_RELEASE(m_pApplyShadow);
	SAFE_RELEASE(m_pCreateShadow);
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pDisc);
	SAFE_RELEASE(m_pShadowDepthStencil);
	//SAFE_RELEASE(m_pDMTexture);
	//SAFE_RELEASE(m_pSMTexture);
}

void cShader::SetupShadow(D3DXVECTOR3 * pvEye, char * szApplyFileName, char * szCreateFileName, char* szMeshFileName)
{
	string sFileName = szApplyFileName;

	string sFullPath = m_sFolder + sFileName;

	char* szFullPath = (char*)sFullPath.c_str();

	m_pApplyShadow = LoadEffect(szFullPath);

	sFileName = szCreateFileName;

	sFullPath = m_sFolder + sFileName;

	szFullPath = (char*)sFullPath.c_str();

	m_pCreateShadow = LoadEffect(szFullPath);

	sFileName = szMeshFileName;

	sFullPath = m_sFolder + sFileName;

	szFullPath = (char*)sFullPath.c_str();

	D3DXLoadMeshFromX(szFullPath, D3DXMESH_MANAGED, g_pD3DDevice, NULL, NULL, NULL, NULL, &m_pMesh);

	D3DXLoadMeshFromX("shader/disc.x", D3DXMESH_MANAGED, g_pD3DDevice, NULL, NULL, NULL, NULL, &m_pDisc);

	m_pvCameraPos = pvEye;

	const int shadowMapSize = 2048;

	g_pD3DDevice->CreateTexture(shadowMapSize, shadowMapSize, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &m_pShadowRenderTarget, NULL);

	g_pD3DDevice->CreateDepthStencilSurface(shadowMapSize, shadowMapSize, D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pShadowDepthStencil, NULL);
}

void cShader::Setup(D3DXVECTOR3* pvEye, char * szFxFileName, char * szMeshFileName, char* szDMTextureFileName, char* szSMTextureFileName, char* szNMTextureFileName, char* szEnvTextureFileName)
{
	string sFileName = szFxFileName;

	string sFullPath = m_sFolder + sFileName;

	char* szFullPath = (char*)sFullPath.c_str();

	m_pApplyShadow = LoadEffect(szFullPath);

	sFileName = szMeshFileName;

	sFullPath = m_sFolder + sFileName;

	szFullPath = (char*)sFullPath.c_str();

	D3DXLoadMeshFromX(szFullPath, D3DXMESH_MANAGED, g_pD3DDevice, NULL, NULL, NULL, NULL, &m_pMesh);

	if (szDMTextureFileName)
	{
		sFileName = szDMTextureFileName;

		sFullPath = m_sFolder + sFileName;

		szFullPath = (char*)sFullPath.c_str();

		m_pDMTexture = g_pTextureManager->GetTexture(szFullPath);
	}

	if (szSMTextureFileName)
	{
		sFileName = szSMTextureFileName;

		sFullPath = m_sFolder + sFileName;

		szFullPath = (char*)sFullPath.c_str();

		m_pSMTexture = g_pTextureManager->GetTexture(szFullPath);
	}

	if (szNMTextureFileName)
	{
		sFileName = szNMTextureFileName;

		sFullPath = m_sFolder + sFileName;

		szFullPath = (char*)sFullPath.c_str();

		m_pNMTexture = g_pTextureManager->GetTexture(szFullPath);
	}

	if (szEnvTextureFileName)
	{
		sFileName = szEnvTextureFileName;

		sFullPath = m_sFolder + sFileName;

		szFullPath = (char*)sFullPath.c_str();

		D3DXCreateCubeTextureFromFile(g_pD3DDevice, szFullPath, &m_pEnvTexture);
	}

	m_pvCameraPos = pvEye;
}

void cShader::Render()
{
	//광원-뷰 행렬을 만든다
	D3DXMATRIXA16 matLightView;
	{
		D3DXVECTOR3 vEyePt(m_vLightPos.x, m_vLightPos.y, m_vLightPos.z);

		D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);

		D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);

		D3DXMatrixLookAtLH(&matLightView, &vEyePt, &vLookatPt, &vUpVec);
	}

	//광원-투영 행렬을 만든다
	D3DXMATRIXA16 matLightProjection;
	{
		D3DXMatrixPerspectiveFovLH(&matLightProjection, D3DX_PI / 4.0f, 1, 1, 10000);
	}

	//뷰-투영 행렬을 만든다
	D3DXMATRIXA16 matViewProjection;
	{
		//뷰 행렬을 만든다
		D3DXMATRIXA16 matView, matProjection;

		g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
		g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProjection);

		matViewProjection = matView * matProjection;
	}

	//월드 행렬을 만든다
	D3DXMATRIXA16 matWorld;
	{
		//프레임마다 0.4도씩 회전시킨다
		m_fRotationY += 0.4f * D3DX_PI / 180.0f;
		if (m_fRotationY > D3DX_PI * 2)
		{
			m_fRotationY -= D3DX_PI * 2;
		}
		D3DXMatrixRotationY(&matWorld, m_fRotationY);
	}

	//디스크의 월드 행렬을 만든다
	D3DXMATRIXA16 matDiscWorld;
	{
		D3DXMATRIXA16 matS, matT;

		D3DXMatrixScaling(&matS, 2, 2, 2);
		D3DXMatrixTranslation(&matT, 0, -40, 0);

		matDiscWorld = matS * matT;
	}

	//현재 하드웨어 백버퍼와 깊이버퍼
	LPDIRECT3DSURFACE9 pHWBackBuffer = NULL;
	LPDIRECT3DSURFACE9 pHWDepthStencilBuffer = NULL;

	g_pD3DDevice->GetRenderTarget(0, &pHWBackBuffer);
	g_pD3DDevice->GetDepthStencilSurface(&pHWDepthStencilBuffer);

	// 1. 그림자 만들기
	LPDIRECT3DSURFACE9 pShadowSurface = NULL;

	if (SUCCEEDED(m_pShadowRenderTarget->GetSurfaceLevel(0, &pShadowSurface)))
	{
		g_pD3DDevice->SetRenderTarget(0, pShadowSurface);
		SAFE_RELEASE(pShadowSurface);
	}

	g_pD3DDevice->SetDepthStencilSurface(m_pShadowDepthStencil);

	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);

	m_pCreateShadow->SetMatrix("matWorld", &matWorld);
	m_pCreateShadow->SetMatrix("matLightView", &matLightView);
	m_pCreateShadow->SetMatrix("matLightProjection", &matLightProjection);

	m_pCreateShadow->SetVector("vLightPos", &m_vLightPos);

	UINT numPasses = 0;
	m_pCreateShadow->Begin(&numPasses, NULL);

	for (int i = 0; i < numPasses; i++)
	{
		m_pCreateShadow->BeginPass(i);

		m_pMesh->DrawSubset(0);

		m_pCreateShadow->EndPass();
	}

	m_pCreateShadow->End();

	//2. 그림자 입히기
	g_pD3DDevice->SetRenderTarget(0, pHWBackBuffer);
	g_pD3DDevice->SetDepthStencilSurface(pHWDepthStencilBuffer);

	SAFE_RELEASE(pHWBackBuffer);
	SAFE_RELEASE(pHWDepthStencilBuffer);

	m_pApplyShadow->SetMatrix("matWorld", &matWorld);
	m_pApplyShadow->SetMatrix("matLightView", &matLightView);
	m_pApplyShadow->SetMatrix("matLightProjection", &matLightProjection);
	m_pApplyShadow->SetMatrix("matViewProjection", &matViewProjection);

	m_pApplyShadow->SetVector("vLightPos", &m_vLightPos);
	m_pApplyShadow->SetVector("vObjectColor", &m_vTorusColor);

	m_pApplyShadow->SetTexture("ShadowMap_Tex", m_pShadowRenderTarget);

	numPasses = 0;
	m_pApplyShadow->Begin(&numPasses, NULL);

	for (int i = 0; i < numPasses; i++)
	{
		m_pApplyShadow->BeginPass(i);

		m_pMesh->DrawSubset(0);

		m_pApplyShadow->SetMatrix("matWorld", &matDiscWorld);
		m_pApplyShadow->SetVector("vObjectColor", &m_vDiscColor);

		m_pApplyShadow->CommitChanges();
		m_pDisc->DrawSubset(0);

		m_pApplyShadow->EndPass();
	}

	m_pApplyShadow->End();
}

LPD3DXEFFECT cShader::LoadEffect(char * szFileName)
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
