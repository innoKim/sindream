#pragma once

class cShader;

class cMainGame
{
private:
	cShader* m_pShader;

public:
	cMainGame();
	~cMainGame();

	void Setup();
	void Update();
	void Render();
};
