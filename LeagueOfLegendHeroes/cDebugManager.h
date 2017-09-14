#pragma once

#define g_pDebug cDebugManager::GetInstance()

class cDebugManager
{
private:
	SINGLETON(cDebugManager);

private:
	ID3DXFont* m_pFont;
	int m_nMax_X;
	int m_nMax_Y;

public:
	void Setup();
	void Destroy();

	void Print(char* str, int nX = 0, int nY = 0, D3DXCOLOR color = D3DCOLOR_XRGB(255, 255, 255));
};

