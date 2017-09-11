#include "stdafx.h"
#include "cShader.h"


cShader::cShader()
	: m_pEffect(NULL)
	, m_pMesh(NULL)
	, m_pDMTexture(NULL)
	, m_pSMTexture(NULL)
	, m_pvCameraPos(NULL)
	, m_vLightColor(D3DXVECTOR4(0.7f, 0.7f, 1.0f, 1.0f))
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
	m_pEffect = LoadEffect(szFxFileName);

	D3DXLoadMeshFromX(szMeshFileName, D3DXMESH_MANAGED, g_pD3DDevice, NULL, NULL, NULL, NULL, &m_pMesh);

	if (szDMTextureFileName)
	{
		m_pDMTexture = g_pTextureManager->GetTexture(szDMTextureFileName);
	}

	if (szSMTextureFileName)
	{
		m_pSMTexture = g_pTextureManager->GetTexture(szSMTextureFileName);
	}

	m_pvCameraPos = pvEye;
	m_vLightPos = D3DXVECTOR4(500.0f, 500.0f, -500.0f, 1.0f);
}

void cShader::Render()
{
	D3DXMATRIXA16 matS, matWorld, matView, matProjection;

	D3DXMatrixScaling(&matS, 0.03f, 0.03f, 0.03f);
	
	matWorld = matS;

	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProjection);

	D3DXVECTOR4 vCameraPos = D3DXVECTOR4(m_pvCameraPos->x, m_pvCameraPos->y, m_pvCameraPos->z, 1.0f);

	// ���̴� ������������ ����
	m_pEffect->SetMatrix("matWorld", &matWorld);
	m_pEffect->SetMatrix("matView", &matView);
	m_pEffect->SetMatrix("matProjection", &matProjection);
	m_pEffect->SetVector("vLightPos", &m_vLightPos);
	m_pEffect->SetVector("vCameraPos", &vCameraPos);

	if (m_pDMTexture)
	{
		m_pEffect->SetTexture("DiffuseMap_Tex", m_pDMTexture);
		m_pEffect->SetVector("vLightColor", &m_vLightColor);
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

	// ���̴� �ε�
	LPD3DXBUFFER		pError = NULL;			//���� ���� ( ���̴��� �������Ҷ� �߸� �� �����̳� ���������� �������ִ� ���� )
	DWORD				dwShaderFlag = 0;		//���̴� �÷��� 0 

#ifdef _DEBUG
	dwShaderFlag = dwShaderFlag | D3DXSHADER_DEBUG;		//���̴��� ����׸��� �������ϰڴ� ( ����׸��� �ؾ� �߸��� ������ ������ ���� Error ���ۿ� ���������� ���� ) 
#endif

														//fx ���Ϸ� ���� ���̴� ��ü ����
	D3DXCreateEffectFromFile(
		g_pD3DDevice,				// ����̽�
		szFileName,					// �ҷ��� ���̴� �ڵ� �����̸�
		NULL,						// ���̴��� �������Ҷ� �߰��� ����� #define ���� ( �ϴ� NULL )
		NULL,						// ���̴��� �������Ҷ� #include ���ù��� ó���Ҷ� ����� �������̽� �÷��� ( �ϴ� NULL )
		dwShaderFlag,				// ���̴� ������ �÷���
		NULL,						// ���̴� �Ű������� ������ �޸�Ǯ ( �ϴ� NULL )
		&pEffect,					// �ε��� ���̴� Effect ������
		&pError						// ���̴��� �ε��ϰ� �������Ҷ� ������ ����� �ش� ���ۿ� �����޽����� ���� ( ���������� �ε��Ǹ� NULL �� �����ȴ� )
	);

	//���̴� ���� �ε��� ������ �ִٸ�..
	if (pError != NULL || pEffect == NULL) {

		//������ ������ ���� ���ڿ��� Ȯ��
		int size = pError->GetBufferSize();
		char* str = new char[size];

		//str�� ���ۿ��ִ� ������ �����Ѵ�.
		sprintf_s(str, size, (const char*)pError->GetBufferPointer());

		OutputDebugString(str);
		//���������� ��������� �������� ����
		SAFE_RELEASE(pError);
		SAFE_DELETE_ARRAY(str);

		return NULL;
	}

	return pEffect;
}
