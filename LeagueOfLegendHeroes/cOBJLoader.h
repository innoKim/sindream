#pragma once
#include "cGroup.h"

#define RATIO	0.5f
#define GRID	10

struct stGrid
{
	D3DXVECTOR3				vecLT;
	D3DXVECTOR3				vecRT;
	D3DXVECTOR3				vecLB;
	D3DXVECTOR3				vecRB;

	vector<D3DXVECTOR3>	vecVertex;
};

class cOBJLoader
{
private:
	float minY, maxY;

	stGrid	m_stGrid[GRID*GRID];

public:
	cOBJLoader();
	~cOBJLoader();

	void Load(IN char* Folder, char* FilePath, OUT vector<cGroup*> &vecGroup, OUT map<string, cMtlTex*>& mapMtlTex);
	void LoadSur(IN char* Filepath, OUT vector<ST_PC_VERTEX> &vecSur, vector<stGrid> &vecGrid);

	void CreateGrid();

	void DivideVertex(stGrid &Grid, vector<ST_PC_VERTEX> vecSur);
protected:
	void LoadMtl(IN char* Folder, IN char* FilePath, OUT map<string, cMtlTex*>& mapMtlTex);
};