#pragma once
#include "cMtlTex.h"

class cGroup
{
private:
	SYNTHESIZE(D3DXMATRIXA16, matWorld, World);
	//SYNTHESIZE_ADD_REF(LPD3DXMESH, m_pMesh, Mesh);
	SYNTHESIZE_PASS_BY_REF(vector<ST_PT_VERTEX>, m_vecVertex, Vertex);
	SYNTHESIZE_ADD_REF(cMtlTex*, m_pMtlTex, MtlTex);
	SYNTHESIZE(int, m_nSubset, Subset);

public:
	cGroup();
	~cGroup();
};