#pragma once
#include "cMtlTex.h"

class cGroup
{
private:
	SYNTHESIZE_PASS_BY_REF(vector<ST_PT_VERTEX>, m_vecVertex, Vertex);
	SYNTHESIZE_ADD_REF(cMtlTex*, m_pMtlTex, MtlTex);

public:
	cGroup();
	~cGroup();

	void Render();
};