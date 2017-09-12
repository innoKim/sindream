#include "stdafx.h"
#include "cShader.h"


cShader::cShader()
	: m_pEffect(NULL)
	, m_pMesh(NULL)
	, m_pDMTexture(NULL)
	, m_pSMTexture(NULL)
	, m_pNMTexture(NULL)
	, m_pEnvTexture(NULL)
	, m_pvCameraPos(NULL)
	, m_vLightColor(D3DXVECTOR4(0.7f, 0.7f, 1.0f, 1.0f))
	, m_sFolder("shader/")
	, m_fRotationY(0.0f)
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

void cShader::Setup(D3DXVECTOR3* pvEye, char * szFxFileName, char * szMeshFileName, char* szDMTextureFileName, char* szSMTextureFileName, char* szNMTextureFileName, char* szEnvTextureFileName)
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
	m_vLightPos = D3DXVECTOR4(-500.0f, 500.0f, -500.0f, 1.0f);
}

void cShader::Render()
{
	D3DXMATRIXA16 matS, matR, matWorld, matView, matProjection, matViewProjection, matWolrdViewProjection, matWorldInverse;

	// �����Ӹ��� 0.4���� ȸ���� ��Ų��.
	//m_fRotationY += 0.4f * D3DX_PI / 180.0f;
	//if (m_fRotationY > 2 * D3DX_PI)
	//{
	//	m_fRotationY -= 2 * D3DX_PI;
	//}

	//D3DXMatrixRotationX(&matR, -D3DX_PI / 2.0f);

	D3DXMatrixIdentity(&matWorld);

	//D3DXMatrixInverse(&matWorldInverse, 0, &matWorld);

	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProjection);

	matViewProjection = matView * matProjection;
	matWolrdViewProjection = matWorld * matView * matProjection;

	D3DXVECTOR4 vCameraPos = D3DXVECTOR4(m_pvCameraPos->x, m_pvCameraPos->y, m_pvCameraPos->z, 1.0f);
	D3DXVECTOR4 vSurfaceColor = D3DXVECTOR4(0.7f, 0.7f, 1.0f, 1.0f);

	// ���̴� ������������ ����
	m_pEffect->SetMatrix("gWorldViewProjectionMatrix", &matWolrdViewProjection);
	//m_pEffect->SetMatrix("matViewProjection", &matViewProjection);
	m_pEffect->SetMatrix("gWorldMatrix", &matWorld);
	//m_pEffect->SetMatrix("matWorldInverse", &matWorldInverse);
	m_pEffect->SetVector("gWorldLightPosition", &m_vLightPos);
	m_pEffect->SetVector("gWorldCameraPosition", &vCameraPos);
	m_pEffect->SetVector("gLightColor", &vSurfaceColor);

	if (m_pDMTexture)
	{
		m_pEffect->SetTexture("DiffuseMap_Tex", m_pDMTexture);
	}

	if (m_pSMTexture)
	{
		m_pEffect->SetTexture("SpecularMap_Tex", m_pSMTexture);
	}

	if (m_pNMTexture)
	{
		m_pEffect->SetTexture("NormalMap_Tex", m_pNMTexture);
	}

	if (m_pEnvTexture)
	{
		m_pEffect->SetTexture("EnvironmentMap_Tex", m_pEnvTexture);
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
