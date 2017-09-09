#include "stdafx.h"
#include "cShader.h"


cShader::cShader()
	: m_pEffect(NULL)
	, m_pMesh(NULL)
{
}


cShader::~cShader()
{
	SAFE_RELEASE(m_pEffect);
	SAFE_RELEASE(m_pMesh);
}

void cShader::Setup(char * szFxFileName, char * szMeshFileName)
{
	m_pEffect = LoadEffect(szFxFileName);

	D3DXLoadMeshFromX(szMeshFileName, D3DXMESH_MANAGED, g_pD3DDevice, NULL, NULL, NULL, NULL, &m_pMesh);

	m_vCameraPos = D3DXVECTOR4(0.0f, 0.0f, -200.0f, 1.0f);
	m_vLightPos = D3DXVECTOR4(500.0f, 500.0f, -500.0f, 1.0f);
}

void cShader::Render()
{
	D3DXMATRIXA16 matWorld, matView, matProjection;

	D3DXMatrixIdentity(&matWorld);

	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProjection);

	D3DXVECTOR3 vCameraPos;

	// ���̴� ������������ ����
	m_pEffect->SetMatrix("matWorld", &matWorld);
	m_pEffect->SetMatrix("matView", &matView);
	m_pEffect->SetMatrix("matProjection", &matProjection);
	m_pEffect->SetVector("vLightPos", &m_vLightPos);
	m_pEffect->SetVector("vCameraPos", &m_vCameraPos);
	

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
