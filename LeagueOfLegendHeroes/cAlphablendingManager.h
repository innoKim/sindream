#pragma once

#define g_pAlphablending cAlphablendingManager::GetInstance()

class cAlphablending;

class cAlphablendingManager
{
private:
	SINGLETON(cAlphablendingManager);
	
private:
	map<string, cAlphablending*>	m_mapAlphablending;

public:
	cAlphablending*	AddAlphablending(string sTag, string sTextureFileName, D3DXVECTOR3 vPos, D3DXVECTOR3 vDir, float fLifeTime, int nSize, bool bBlending = false, bool bBillboarding = false);
	cAlphablending*	GetAlphablending(string sTag);
	void			Play(string sTag);
	void			Destroy();
	void			Update();
	void			UIRender();
};
