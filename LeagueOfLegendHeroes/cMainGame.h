#pragma once

class cShader;
class cCamera;

class cMainGame
{
private:
	cCamera*	m_pCamera;
	cShader*	m_pShader;

public:
	cMainGame();
	~cMainGame();

	void Setup();
	void Update();
	void Render();
	void MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
