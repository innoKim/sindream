#pragma once
#include "cGroup.h"
#include "cOBJLoader.h"

#define INFH	FLT_MAX

class cUnit;

class cMap
{
private:
	LPD3DXMESH						m_pMesh;
	vector<cGroup*>					m_vecMap;
	vector<ST_PC_VERTEX>			m_vecSur;
	vector<stGrid>					m_vecGrid;
	map<string, cMtlTex*>			m_mapMtlTex;
	map<string, cMtlTex*>::iterator	m_iterMtlTex;
	vector<cMtlTex*>				m_vecMtlTex;

	D3DXMATRIX						m_matW, m_matS, m_matR, m_matT;

	RayInfo							m_stRay;

	HitInfo							m_stHit;

	float							m_fRayDist;

public:
	cMap();
	~cMap();

	void LoadMap(IN char* Folder, IN char* File);
	void LoadSur(char* FileFullPath);

	float GetGroundHeight(D3DXVECTOR3 pos);
};

