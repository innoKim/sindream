#pragma once
#include "cOBJLoader.h"

class cShader;
class cCamera;

class cMainGame
{
private:
	cCamera*	m_pCamera;
	cShader*	m_pShader;

	vector<ST_PC_VERTEX>	m_vecVertex;

public:
	cMainGame();
	~cMainGame();

	void Setup();
	void Update();
	void Render();
	void MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
