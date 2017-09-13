#pragma once

class iScene;

class cMainGame
{
private:
	iScene* m_pScene;

public:
	cMainGame();
	~cMainGame();

	void Setup();
	void Update();
	void Render();
};
