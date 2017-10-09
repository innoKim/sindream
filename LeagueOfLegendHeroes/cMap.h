#pragma once
#include "cOBJLoader.h"

#define INFH	(-1000.0f)

class cUnit;
class cGroup;
class cAStarGrid;

class cMap
{
private:
	LPD3DXMESH						m_pMesh;

	vector<cGroup*>					m_vecMap;
	vector<ST_PC_VERTEX>			m_vecSur;

	map<string, cMtlTex*>			m_mapMtlTex;
	map<string, cMtlTex*>::iterator	m_iterMtlTex;

	vector<cMtlTex*>				m_vecMtlTex;

	D3DXMATRIX						m_matW, m_matS, m_matR, m_matT;
	
	SYNTHESIZE_PASS_BY_REF(vector<vector<ST_HEIGHTNODE>>, m_gridHeightNode, HeightNode); // 잘 나눠져서 들어왔네요
	
	SYNTHESIZE(cAStarGrid*,			m_pAStarGrid, Grid);
public:
	cMap();
	~cMap();

	void LoadMap(IN char* Folder, IN char* File);
	void LoadSur(char* FileFullPath);

	float GetHeight(D3DXVECTOR3 pos);
};

