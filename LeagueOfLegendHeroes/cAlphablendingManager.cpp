#include "stdafx.h"
#include "cAlphablending.h"
#include "cAlphablendingManager.h"

cAlphablendingManager::cAlphablendingManager()
{
}


cAlphablendingManager::~cAlphablendingManager()
{
}

cAlphablending* cAlphablendingManager::AddAlphablending(string sTag, string sTextureFileName, D3DXVECTOR3 vPos, D3DXVECTOR3 vDir, float fLifeTime, int nSize, bool bBlending, bool bBillboarding)
{
	if (m_mapAlphablending.find(sTag) != m_mapAlphablending.end()) return NULL;

	cAlphablending* pAlphablending = new cAlphablending;
	pAlphablending->Setup(sTag, sTextureFileName, vPos, vDir, fLifeTime, nSize, bBlending, bBillboarding);

	m_mapAlphablending.insert(make_pair(sTag, pAlphablending));

	return pAlphablending;
}

cAlphablending * cAlphablendingManager::GetAlphablending(string sTag)
{
	return m_mapAlphablending[sTag];
}

void cAlphablendingManager::Play(string sTag)
{
	m_mapAlphablending[sTag]->Play();
}

void cAlphablendingManager::Destroy()
{
	for each(auto it in m_mapAlphablending)
	{
		SAFE_DELETE(it.second);
	}
}

void cAlphablendingManager::Update()
{
	for each (auto it in m_mapAlphablending)
	{
		(it.second)->Update();
	}
}

void cAlphablendingManager::UIRender()
{
	for each (auto it in m_mapAlphablending)
	{
		(it.second)->UIRender();
	}
}
