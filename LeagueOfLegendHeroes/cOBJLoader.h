#pragma once
#include "cGroup.h"

#define MAP_SIZE	15000
#define MAP_RATIO	0.5f
#define MAP_GRID	5

//struct stGrid
//{
//	D3DXVECTOR3				vecLT;
//	D3DXVECTOR3				vecRT;
//	D3DXVECTOR3				vecLB;
//	D3DXVECTOR3				vecRB;
//
//	vector<D3DXVECTOR3>	vecVertex;
//};

struct ST_HEIGHTNODE
{
	int xIndex;
	int zIndex;
	
	ST_SQUARE sq;

	vector<D3DXVECTOR3>	vecVertex;
};

class cOBJLoader
{	
public:
	cOBJLoader();
	~cOBJLoader();

	void Load(IN char* Folder, char* FilePath, OUT vector<cGroup*> &vecGroup, OUT map<string, cMtlTex*>& mapMtlTex);
	void LoadSur(IN char* Filepath, OUT vector<ST_PC_VERTEX> &vecSur, vector<vector<ST_HEIGHTNODE>>& gridHeightNode );



	//void CreateGrid();
	//void DivideVertex(stGrid &Grid, vector<ST_PC_VERTEX> vecSur);
protected:
	void LoadMtl(IN char* Folder, IN char* FilePath, OUT map<string, cMtlTex*>& mapMtlTex);
	void SortTriangleToGrid(vector<vector<ST_HEIGHTNODE>>& gridHeightNode, D3DXVECTOR3 & a, D3DXVECTOR3 & b, D3DXVECTOR3 & c);
};