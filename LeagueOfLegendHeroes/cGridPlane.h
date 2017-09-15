#pragma once
class cGridPlane
{
private:
	vector<ST_PC_VERTEX> m_vecVertex;

public:
	cGridPlane();
	virtual ~cGridPlane();

	virtual void Setup(int size,int nodeSize);
	virtual void Render();
};

