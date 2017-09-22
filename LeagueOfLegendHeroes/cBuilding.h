#pragma once
#include "cUnit.h"
class cBuilding : public cUnit
{
public:
	cBuilding();
	~cBuilding();

	virtual void Update() override;

	void KeyControl();
};

