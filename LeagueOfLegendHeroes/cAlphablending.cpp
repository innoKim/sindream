#include "stdafx.h"
#include "cAlphablending.h"


cAlphablending::cAlphablending()
	: m_bBillboarding(false)
	, m_bBlending(false)
	, m_fLifeTime(0.0f)
	, m_fCurrentTime(0.0f)
	, m_pTexture(NULL)
	, m_vPos(D3DXVECTOR3(0, 0, 0))
	, m_vDir(D3DXVECTOR3(0, 0, 0))
	, m_bEnd(true)
{
}


cAlphablending::~cAlphablending()
{
}

void cAlphablending::Setup(string sTag, string sTextureFileName, D3DXVECTOR3 vPos, D3DXVECTOR3 vDir, float fLifeTime, int nSize, bool bBlending, bool bBillboarding)
{
	m_sTag = sTag;

	string sFolder = "texture/";
	string sFileName = sTextureFileName;

	string sFullPath = sFolder + sFileName;

	m_pTexture = g_pTextureManager->GetTexture(sFullPath.c_str());

	D3DXVec3Normalize(&m_vDir, &vDir);
	m_vPos = vPos;

	m_fLifeTime = fLifeTime;
	m_nSize = nSize;

	m_bBlending = bBlending;
	m_bBillboarding = bBillboarding;

	m_vecVertex.resize(6);

	m_vecVertex[0].p = D3DXVECTOR3( m_nSize / 2.0f, m_nSize / 2.0f, 0);
	m_vecVertex[0].t = D3DXVECTOR2(0, 0);
	m_vecVertex[1].p = D3DXVECTOR3(-m_nSize / 2.0f, m_nSize / 2.0f, 0);
	m_vecVertex[1].t = D3DXVECTOR2(1, 0);
	m_vecVertex[2].p = D3DXVECTOR3(-m_nSize / 2.0f, -m_nSize / 2.0f, 0);
	m_vecVertex[2].t = D3DXVECTOR2(1, 1);
	m_vecVertex[3].p = D3DXVECTOR3( m_nSize / 2.0f, m_nSize / 2.0f, 0);
	m_vecVertex[3].t = D3DXVECTOR2(0, 0);
	m_vecVertex[4].p = D3DXVECTOR3(-m_nSize / 2.0f, -m_nSize / 2.0f, 0);
	m_vecVertex[4].t = D3DXVECTOR2(1, 1);
	m_vecVertex[5].p = D3DXVECTOR3( m_nSize / 2.0f, -m_nSize / 2.0f, 0);
	m_vecVertex[5].t = D3DXVECTOR2(0, 1);
}

void cAlphablending::Update()
{
	if (!m_bEnd)
	{
		if (m_fCurrentTime < m_fLifeTime)
		{
			m_fCurrentTime += g_pTimeManager->GetElapsedTime();
		}
		else
		{
			m_bEnd = true;
		}
	}
}

void cAlphablending::UIRender()
{
	if (!m_bEnd)
	{
		D3DXMATRIXA16 matR, matRX, matRY, matRZ, matT, matWorld;

		D3DXVECTOR3 vUp = D3DXVECTOR3(0, 0.99, 0.01);

		D3DXVec3Normalize(&vUp, &vUp);

		D3DXMatrixLookAtLH(&matR, &D3DXVECTOR3(0, 0, 0), &m_vDir, &vUp);
		D3DXMatrixTranspose(&matR, &matR);
		//D3DXMatrixRotationX(&matRX, m_vDir.x * D3DX_PI / 2.0f);
		//D3DXMatrixRotationY(&matRY, m_vDir.y * D3DX_PI / 2.0f);
		//D3DXMatrixRotationZ(&matRZ, m_vDir.z * D3DX_PI / 2.0f);

		//matR = matRX * matRY * matRZ;

		D3DXMatrixTranslation(&matT, m_vPos.x, m_vPos.y + 1, m_vPos.z);

		matWorld = matR * matT;

		g_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

		g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);

		if (m_bBlending)
		{
			g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
			g_pD3DDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DXCOLOR(1.0f, 1.0f, 1.0f, (m_fLifeTime - m_fCurrentTime) / m_fLifeTime));
		}

		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
		g_pD3DDevice->SetTexture(0, m_pTexture);
		g_pD3DDevice->SetFVF(ST_PT_VERTEX::FVF);
		g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vecVertex.size() / 3, &m_vecVertex[0], sizeof(ST_PT_VERTEX));
		g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);

		g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	}
}
