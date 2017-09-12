#pragma once
#include "cOBJLoader.h"
#include "cGroup.h"

class cShader;
class cCamera;

class cMainGame
{
private:
	cCamera*		m_pCamera;
	cShader*		m_pShader;
	
	vector<cGroup*>	m_vecGroup;

	D3DXMATRIX		m_matW, m_matS, m_matR, m_matT;

	vector<ST_PC_VERTEX> m_vecSur;

	// 테스트용
	D3DXVECTOR3		m_vPos;
	D3DXVECTOR3		m_vDir;
	float			m_fAngle;
	float			m_fSpeed;

public:
	cMainGame();
	~cMainGame();

	void Setup();
	void Update();
	void Render();
	void MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
