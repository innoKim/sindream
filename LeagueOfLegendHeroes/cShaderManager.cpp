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
	//���� �ϵ���� ����ۿ� ���̹���
	g_pD3DDevice->GetRenderTarget(0, &m_pHWBackBuffer);
	g_pD3DDevice->GetDepthStencilSurface(&m_pHWDepthStencilBuffer);

	// 1. �׸��� �����
	LPDIRECT3DSURFACE9 pShadowSurface = NULL;

	if (SUCCEEDED(m_pShadowRenderTarget->GetSurfaceLevel(0, &pShadowSurface)))
	{
		g_pD3DDevice->SetRenderTarget(0, pShadowSurface);
		SAFE_RELEASE(pShadowSurface);
	}

	g_pD3DDevice->SetDepthStencilSurface(m_pShadowDepthStencil);

	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);

	//����-�� ����� �����
	D3DXVECTOR3 vEyePt(m_pvTarget->x + m_vLightPos.x, m_pvTarget->y + m_vLightPos.y, m_pvTarget->z + m_vLightPos.z);

	D3DXVECTOR3 vLookatPt(0, 0, 0);

	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);

	D3DXMatrixLookAtLH(&m_matLightView, &vEyePt, &vLookatPt, &vUpVec);

	//����-���� ����� �����
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
	//2. �׸��� ������
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
