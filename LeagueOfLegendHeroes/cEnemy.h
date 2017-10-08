#pragma once
#include "cUnit.h"

class cAStar;
class cAStarGrid;
class cAStarNode;

class cEnemy : public cUnit
{
public :
	enum eEnemyType
	{
		ENEMYTYPE_MELEE
	};

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

	virtual void Setup(eEnemyType enemyType, cMap* mapPtr);
	virtual void Update(D3DXVECTOR3 vPlayerPos);
	virtual void Render() override;
};

