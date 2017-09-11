#include "stdafx.h"
#include "cShader.h"


cShader::cShader()
	: m_pEffect(NULL)
	, m_pMesh(NULL)
	, m_pDMTexture(NULL)
	, m_pSMTexture(NULL)
	, m_pvCameraPos(NULL)
	, m_vLightColor(D3DXVECTOR4(0.7f, 0.7f, 1.0f, 1.0f))
	, m_sFolder("shader/")
{
}


cShader::~cShader()
{
	m_pvCameraPos = NULL;

	SAFE_RELEASE(m_pEffect);
	SAFE_RELEASE(m_pMesh);
	//SAFE_RELEASE(m_pDMTexture);
	//SAFE_RELEASE(m_pSMTexture);
}

void cShader::Setup(D3DXVECTOR3* pvEye, char * szFxFileName, char * szMeshFileName, char* szDMTextureFileName, char* szSMTextureFileName)
{
	string sFileName = szFxFileName;

	string sFullPath = m_sFolder + sFileName;

	char* szFullPath = (char*)sFullPath.c_str();

	m_pEffect = LoadEffect(szFullPath);

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
		sFileName = szDMTextureFileName;

		sFullPath = m_sFolder + sFileName;

		szFullPath = (char*)sFullPath.c_str();

		m_pSMTexture = g_pTextureManager->GetTexture(szFullPath);
	}

	m_pvCameraPos = pvEye;
	m_vLightPos = D3DXVECTOR4(500.0f, 500.0f, -500.0f, 1.0f);
}

void cShader::Render()
{
	D3DXMATRIXA16 matS, matR, matWorld, matView, matProjection, matWolrdViewProjection, matInvWorld;

	D3DXMatrixScaling(&matS, 0.03f, 0.03f, 0.03f);
	D3DXMatrixRotationX(&matR, -D3DX_PI / 2.0f);

	matWorld = matS * matR;

	D3DXMatrixInverse(&matInvWorld, 0, &matWorld);

	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProjection);

	matWolrdViewProjection = matWorld * matView * matProjection;

	D3DXVECTOR4 vCameraPos = D3DXVECTOR4(m_pvCameraPos->x, m_pvCameraPos->y, m_pvCameraPos->z, 1.0f);
	D3DXVECTOR4 vSurfaceColor = D3DXVECTOR4(0, 1, 0, 1);

	// 쉐이더 전역변수들을 설정
	m_pEffect->SetMatrix("matWorldViewProjection", &matWolrdViewProjection);
	m_pEffect->SetMatrix("matWorldInverse", &matInvWorld);
	m_pEffect->SetVector("LightPos", &m_vLightPos);
	//m_pEffect->SetVector("vCameraPos", &vCameraPos);
	//m_pEffect->SetVector("SurfaceColor", &vSurfaceColor);

	if (m_pDMTexture)
	{
		m_pEffect->SetTexture("DiffuseMap_Tex", m_pDMTexture);
	}

	if (m_pSMTexture)
	{
		m_pEffect->SetTexture("SpecularMap_Tex", m_pSMTexture);
	}


	UINT numPasses = 0;
	m_pEffect->Begin(&numPasses, NULL);

	for (UINT i = 0; i < numPasses; ++i)
	{
		m_pEffect->BeginPass(i);

		m_pMesh->DrawSubset(0);

		m_pEffect->EndPass();
	}

	m_pEffect->End();
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
