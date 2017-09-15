#pragma once

class iScene;
class cGridPlane;

class cMainGame
{
private:
	iScene* m_pScene;
	cGridPlane* m_pGrid;

public:
	cMainGame();
	~cMainGame();

	void Setup();
	void Update();
	void Render();
};
