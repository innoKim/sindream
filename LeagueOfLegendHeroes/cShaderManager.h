#pragma once

#define g_pShaderManager cShaderManager::GetInstance()

class cGroup;
class cMtlTex;

class cShaderManager
{
private:
	SINGLETON(cShaderManager);

private:
	LPD3DXEFFECT				m_pApplyShadow;				//2�н��� ����� ���̴�
	LPD3DXEFFECT				m_pCreateShadow;			//1�н��� ����� ���̴�
	D3DXVECTOR4					m_vLightPos;				//ĳ���͸� �������� �� ������ ����� ��ġ
	D3DXVECTOR4					m_vLightDirection;			//������ Direction
	LPDIRECT3DTEXTURE9			m_pShadowRenderTarget;		//1�н��� ������� �׸��µ� ����� ����Ÿ��
	LPDIRECT3DSURFACE9			m_pShadowDepthStencil;		//������� �׸��µ� ����� ���� ����
	D3DXVECTOR4					m_vLightColor;				//������ ��
	LPDIRECT3DSURFACE9			m_pHWBackBuffer;			//���� ������̴� ����۸� �ӽ� ������ ����
	LPDIRECT3DSURFACE9			m_pHWDepthStencilBuffer;	//���� ������̴� ���� ���۸� �ӽ� ������ ����
	vector<LPD3DXMESH>			m_vecMesh;					//�׸��ڸ� ������ ������Ʈ���� ������ ����
	vector<LPDIRECT3DTEXTURE9>	m_vecTexture;				//�� ������Ʈ�� ����� �ؽ�ó
	vector<D3DXMATRIXA16>		m_vecMatWorld;				//�� ������Ʈ�� ����� ���� ��Ʈ����
	D3DXMATRIXA16				m_matLightView;				//����-�� ��Ʈ����
	D3DXMATRIXA16				m_matLightProjection;		//����-���� ��Ʈ����
	D3DXMATRIXA16				m_matViewProjection;		//��-���� ��Ʈ����
	D3DXVECTOR3*				m_pvTarget;					//ī�޶� ����ٴϴ� Ÿ��
	LPD3DXMESH					m_pMeshGround;				//������ ���� �޽�
	D3DXMATRIXA16				m_matWorldGround;			//�ʿ� ����� ���� ��Ʈ����
	vector<cMtlTex*>			m_vecMtlTex;				//�ʿ� ����� �ؽ�ó
	LPDIRECT3DCUBETEXTURE9		m_pCubeTexture;				//��ī�̹ڽ��� ���� ť�� �ؽ�ó
	LPD3DXEFFECT				m_pSkybox;					//��ī�̹ڽ��� �׸��� ���� ���̴�
	LPD3DXMESH					m_pCube;

public:
	void BeginRender();		//�׸��ڸ� �׸��� ���Ͽ� ������ ������ �ʱ�ȭ�� ���� ȣ��Ǵ� begin�Լ�
	void SetupShadow();		//������ ���� ���� �¾� �Լ�
	void RenderShadow(LPD3DXMESH pMesh, LPDIRECT3DTEXTURE9 pTexture, D3DXMATRIXA16 matWorld);	//������ �ʿ� ���� ��ġ���� ���̴� ������Ʈ�� �Ÿ�(���̰�)���� ������ 1�н� ������
	void Render();	//�׸��ڰ� ������ ������Ʈ���� ���������� �׷��ִ� ���� �Լ�
	void Destroy();
	void SetPlane(LPD3DXMESH pMesh, D3DXMATRIXA16 matWorld);	//�ؽ�ó ���� ���� �޽��� �̷���� �� �¾�
	void SetMap(LPD3DXMESH pMesh, vector<cMtlTex*> vecMtlTex, D3DXMATRIXA16 matWorldGround);	//���� �޽��̵� ���� ���� �ؽ�ó�� �̷���� �� �¾�
	void SetTarget(D3DXVECTOR3* pvTarget) { m_pvTarget = pvTarget; }	//Ÿ�� ����

private:
	LPD3DXEFFECT LoadEffect(char* szFileName);
};

