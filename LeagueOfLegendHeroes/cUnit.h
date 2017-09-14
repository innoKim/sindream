#pragma once

class cSkinnedMesh;
class cPhysics;

enum eSTATE
{
	STATE_IDLE,
	STATE_RUN,
	STATE_ATTACK,
	STATE_SPELL1,
	STATE_SPELL2,
	STATE_SPELL3,
	STATE_SPELL4
};

struct ST_UNITLOADINFO
{
	eSTATE			state;
	const char*		fileName;
	CALLBACKFUNC	callbackFunc;
	void*			callbackObj;
};


class cUnit
{
protected:
	map<eSTATE, cSkinnedMesh*> m_mapStates;
	eSTATE   m_eStateNum;
	cSkinnedMesh* m_pCurState;

	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vPos, Position)
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vDir, Direction)
	SYNTHESIZE(cPhysics*, m_pPhysics, Physics)

	void Destroy();
public:
	cUnit();
	~cUnit();

	virtual void Setup(vector<ST_UNITLOADINFO> statesVector);
	virtual void Update();
	virtual void Render();

	void SetState(eSTATE state);

	bool IsReady() { return ((m_eStateNum == STATE_RUN || m_eStateNum == STATE_IDLE) ? true : false); }

	D3DXVECTOR3* GetPosPtr() { return &m_vPos; }
	void SetPosY(float posY) { m_vPos.y = posY; }

	void SetPhysicsTargets(vector<cPhysics*>* vector);
};