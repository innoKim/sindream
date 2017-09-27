#include "stdafx.h"
#include "cParticleGroup.h"
#include "cParticle.h"

cParticleGroup::cParticleGroup() :
	m_szTexturePath("texture/particleTex.tga"),
	m_nInitParticle(1),
	m_fLifeTime(1.0f),
	m_fLifeTimeVariation(0.0f),
	m_vStartPosition(0, 0, 0),
	m_fStartPositionVariation(0.0f),
	m_vVelocity(0, 0, 0),
	m_fVelocityVariation(0.0f),
	m_fDragVelocity(0.0f),
	m_vAcceleration(0, 0, 0),
	m_fAccelerationVariation(0.0f),
	m_dStartColor(1, 1, 1, 1),
	m_cStartColorVariation(0, 0, 0, 0),
	m_dEndColor(0,0,0,0),
	m_cEndColorVariation(0, 0, 0, 0)
{
}

cParticleGroup::~cParticleGroup()
{
	for each (auto p in m_vecParticle)
	{
		SAFE_DELETE(p);
	}
}

void cParticleGroup::Setup()
{
	while (m_vecVertex.size() < m_nInitParticle)
	{
		AddParticle();
	}
}

void cParticleGroup::Update()
{
	ParticleUpdate();
}

void cParticleGroup::Render()
{
	if (m_vecVertex.empty()) return;


	// 포인트를 확대 축소 할 수 있게 해줌
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALEENABLE, true);

	// 포인트 사이즈 설정
	g_pD3DDevice->SetRenderState(D3DRS_POINTSIZE, MY_UTIL::FtoDw(10.f));

	// 포인트 스케일링 Factor값 설정
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALE_A, MY_UTIL::FtoDw(0.0f));
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALE_B, MY_UTIL::FtoDw(0.0f));
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALE_C, MY_UTIL::FtoDw(1.0f));

	// 포인트 최소 크기
	g_pD3DDevice->SetRenderState(D3DRS_POINTSIZE_MIN, MY_UTIL::FtoDw(0.0f));

	// 포인트 최대 크기
	g_pD3DDevice->SetRenderState(D3DRS_POINTSIZE_MAX, MY_UTIL::FtoDw(100.0f));

	// 포인트에 텍스쳐를 입힐 수 있게 해줌
	g_pD3DDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, true);

	// 텍스쳐 알파 옵션 설정
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	g_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	D3DXMATRIXA16 matWorld;
	D3DXMatrixTranslation(&matWorld, m_vStartPosition.x, m_vStartPosition.y, m_vStartPosition.z);
	
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);

	if (g_pTextureManager->GetTexture(m_szTexturePath))	g_pD3DDevice->SetTexture(0, g_pTextureManager->GetTexture(m_szTexturePath));
	else g_pD3DDevice->SetTexture(0, NULL);

	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_POINTLIST,
	
		m_vecVertex.size(),
		&m_vecVertex[0],
		sizeof(ST_PC_VERTEX));

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

void cParticleGroup::Init()
{	
	m_szTexturePath = "texture/particleTex.tga";
	m_nInitParticle = 1;
	m_fLifeTime = 1.0f;
	m_fLifeTimeVariation=0.0f;
	m_vStartPosition=D3DXVECTOR3(0, 0, 0);
	m_fStartPositionVariation=0.0f;
	m_vVelocity= D3DXVECTOR3(0, 0, 0);
	m_fVelocityVariation=0.0f;
	m_fDragVelocity=0.0f;
	m_vAcceleration= D3DXVECTOR3(0, 0, 0);
	m_fAccelerationVariation=0.0f;
	m_dStartColor=D3DXCOLOR(1, 1, 1, 1);
	m_cStartColorVariation= D3DXCOLOR(0, 0, 0, 0);
	m_dEndColor= D3DXCOLOR(0, 0, 0, 0);
	m_cEndColorVariation= D3DXCOLOR(0, 0, 0, 0);

	for each (auto p in m_vecParticle)
	{
		SAFE_DELETE(p);
	}

	m_vecParticle.clear();
	m_vecVertex.clear();
}

void cParticleGroup::Clone(cParticleGroup * target)
{
	m_szTexturePath            = target->GetTexturePath ();
	m_nInitParticle			   = target->GetInitParticleNumber();
	m_fLifeTime				   = target->GetLifeTime				();  
	m_fLifeTimeVariation	   = target->GetLifeTimeVariation	   ();
	m_vStartPosition		   = target->GetStartPosition		   ();
	m_fStartPositionVariation  = target->GetStartPositionVariation ();
	m_vVelocity				   = target->GetVelocity				();  
	m_fVelocityVariation	   = target->GetVelocityVariation	   ();
	m_fDragVelocity			   = target->GetDragVelocity			();  
	m_vAcceleration			   = target->GetAcceleration			();  
	m_fAccelerationVariation   = target->GetAccelerationVariation  ();
	m_dStartColor			   = target->GetStartColor			   ();
	m_cStartColorVariation	   = target->GetStartColorVariation	   ();
	m_dEndColor				   = target->GetEndColor				();  
	m_cEndColorVariation	   = target->GetEndColorVariation	   ();
}

void cParticleGroup::AddParticle()
{
	m_vecVertex.resize(m_vecVertex.size() + 1);

	D3DXCOLOR StartColorVariation(g_pRND->GetFromFloatTo(-m_cStartColorVariation.r, m_cStartColorVariation.r), g_pRND->GetFromFloatTo(-m_cStartColorVariation.g, m_cStartColorVariation.g), g_pRND->GetFromFloatTo(-m_cStartColorVariation.b, m_cStartColorVariation.b), g_pRND->GetFromFloatTo(-m_cStartColorVariation.a, m_cStartColorVariation.a));
	D3DXCOLOR EndColorVariation(g_pRND->GetFromFloatTo(-m_cEndColorVariation.r, m_cEndColorVariation.r), g_pRND->GetFromFloatTo(-m_cEndColorVariation.g, m_cEndColorVariation.g), g_pRND->GetFromFloatTo(-m_cEndColorVariation.b, m_cEndColorVariation.b), g_pRND->GetFromFloatTo(-m_cEndColorVariation.a, m_cEndColorVariation.a));
	
	cParticle* newParticle = new cParticle;
	newParticle->Setup(g_pRND->GetFromVectorTo(-m_fStartPositionVariation, m_fStartPositionVariation)
		, m_vVelocity + g_pRND->GetFromVectorTo(-m_fVelocityVariation, m_fVelocityVariation)
		, m_vAcceleration + g_pRND->GetFromVectorTo(-m_fAccelerationVariation, m_fAccelerationVariation)
		, m_fLifeTime + g_pRND->GetFromFloatTo(-m_fLifeTimeVariation, m_fLifeTimeVariation)
		, m_dStartColor + StartColorVariation
		, m_dEndColor + EndColorVariation
		, m_fDragVelocity);

	m_vecParticle.push_back(newParticle);
}

void cParticleGroup::ParticleUpdate()
{
	vector<ST_PC_VERTEX>::iterator	itorVertex		= m_vecVertex.begin();
	vector<cParticle*>::iterator	itorParticle	= m_vecParticle.begin();

	for (itorVertex; itorVertex != m_vecVertex.end();)
	{
		if ((*itorParticle)->IsDead())
		{
			SAFE_DELETE((*itorParticle));
			itorParticle = m_vecParticle.erase(itorParticle);
			itorVertex = m_vecVertex.erase(itorVertex);
		}
		else
		{
			(*itorParticle)->Update();
			(*itorVertex).c = (*itorParticle)->GetCurColor();
			(*itorVertex).p = (*itorParticle)->GetCurPosition();
			++itorParticle;
			++itorVertex;
		}
	}
}

