#pragma once
#include "cUnit.h"

class cEnemy : public cUnit
{
private:

public:
	cEnemy();
	virtual ~cEnemy();

	virtual void Update() override;
	virtual void Render() override;
};

