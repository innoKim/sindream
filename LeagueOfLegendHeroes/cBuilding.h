#pragma once
#include "cUnit.h"
class cBuilding : public cUnit
{
private:
	bool	m_bSelect;
	float	m_fSpeed;

public:
	cBuilding();
	~cBuilding();

	virtual void Update() override;

	void KeyControl();
	void SetSelect(bool select) { m_bSelect = select; }
};

