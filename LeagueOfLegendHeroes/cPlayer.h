#pragma once
#include "cUnit.h"

#define PLAYER_ROTSPD 0.05f
#define PLAYER_SPD 0.03f

class cPlayer :
	public cUnit
{
public:
	cPlayer();
	virtual ~cPlayer();
	
	virtual void Update() override;
	virtual void Render() override;

	void KeyControl();
};

