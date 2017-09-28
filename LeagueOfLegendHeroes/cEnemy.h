#pragma once
#include "cUnit.h"

class cAStar;
class cAStarGrid;
class cAStarNode;

class cEnemy : public cUnit
{
private:
	cAStar*						m_pAStar;
	cAStarGrid*					m_pAStarGrid;
	D3DXVECTOR3					m_vTargetPos;
	vector<cAStarNode*>			m_vecPath;
	float						m_fWorldTime;
	bool						m_bTargetMove;

public:
	cEnemy();
	virtual ~cEnemy();

	virtual void Setup(vector<ST_UNITLOADINFO> statesVector, cMap* mapPtr);
	virtual void Update(D3DXVECTOR3 vPlayerPos);
	virtual void Render() override;
};

