#pragma once
#include "cUnit.h"

struct ST_MoveInfo
{
	int nVerticalFactor = 0;
	int nHorizontalFactor = 0;

	D3DXVECTOR3 vVerticalDir;
	D3DXVECTOR3 vHorizontalDir;
	D3DXVECTOR3 vMoveDir;
};

class cPlayer :
	public cUnit
{
private:
	ST_MoveInfo m_moveInfo;

public:
	cPlayer();
	virtual ~cPlayer();
	
	virtual void Update() override;
	virtual void Render() override;

	void KeyControl();
	void FindMoveDir();
};

