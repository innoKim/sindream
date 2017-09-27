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

	
	//����-���� ����� �����
	D3DXMatrixPerspectiveFovLH(&m_matLightProjection, D3DX_PI / 4.0f, 1, 1, 3600);
	m_pCreateShadow->SetMatrix("matLightProjection", &m_matLightProjection);

	if (m_bSkyboxOn)
	{
		m_pSkybox = LoadEffect("shader/Skybox.fx");
		D3DXCreateCubeTextureFromFile(g_pD3DDevice, "shader/cubeTexture1.dds", &m_pCubeTexture);
		D3DXCreateBox(g_pD3DDevice, 20000, 20000, 20000, &m_pCube, NULL);
	}

	//������ �� ����, ���� ������
	const int shadowMapSize = 8192;

	//����Ÿ�� ������ ������ �� ����
	g_pD3DDevice->CreateTexture(shadowMapSize, shadowMapSize, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &m_pShadowRenderTarget, NULL);

	//������ �ʿ��� ���� ���� ���� ����
	g_pD3DDevice->CreateDepthStencilSurface(shadowMapSize, shadowMapSize, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pShadowDepthStencil, NULL);

	//�����׽�Ʈ ����, ���İ��� 0�� ��� �׸��� �ʴ´�
	g_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 0);
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

	m_pApplyShadow->SetFloat("fLightWeight", 1.0f);
	m_pApplyShadow->SetBool("bTexture", true);
}

void cShaderManager::RenderShadow(LPD3DXMESH pMesh, LPDIRECT3DTEXTURE9 pTexture, D3DXMATRIXA16 matWorld)
{
	//2�н� �������� ���� ���Ϳ� �޽��� �ؽ�ó, ���� ��Ʈ�������� ����
	m_vecMesh.push_back(pMesh);
	m_vecTexture.push_back(pTexture);
	m_vecMatWorld.push_back(matWorld);

	//����-�� ����� �����
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

	//���� ���� Ÿ���� ������ ��, ������ �ʿ� ���� ��ġ���� �� ������Ʈ�� ����� �׸���
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
	//���� Ÿ���� ������ �ϵ���� ����ۿ� ���� ���۷� ��ü
	g_pD3DDevice->SetRenderTarget(0, m_pHWBackBuffer);
	g_pD3DDevice->SetDepthStencilSurface(m_pHWDepthStencilBuffer);

	//�� �̻� ������� �ʴ� �ӽ� ���� �޸� ����
	SAFE_RELEASE(m_pHWBackBuffer);
	SAFE_RELEASE(m_pHWDepthStencilBuffer);

	D3DXMATRIXA16 matView, matProjection;

	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProjection);

	//��-���� ��Ʈ����
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
	
	//2�н� �������� �ʿ��� ���� ���� ����
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

	//�׸��� �׸��µ� �ʿ��� ������ �� ����Ÿ�� ����
	m_pApplyShadow->SetTexture("ShadowMap_Tex", m_pShadowRenderTarget);

	numPasses = 0;
	m_pApplyShadow->Begin(&numPasses, NULL);

	for (int i = 0; i < numPasses; i++)
	{
		m_pApplyShadow->BeginPass(i);
		
		//1�н��� ������ �޽��� ������ ���� ��Ʈ������ �ؽ�ó�� ���Ͽ� �׸��ڰ� ������ ���·� ������
		for (int j = 0; j < m_vecMesh.size(); j++)
		{
			m_pApplyShadow->SetMatrix("matWorld", &m_vecMatWorld[j]);
			m_pApplyShadow->SetTexture("DiffuseMap_Tex", m_vecTexture[j]);
			m_pApplyShadow->CommitChanges();

			m_vecMesh[j]->DrawSubset(0);
		}

		//�� ������Ʈ���� �׸��ڰ� ������ �ٴ� �� ������
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
