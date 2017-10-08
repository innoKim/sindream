#pragma once
#include "cUnit.h"

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

