#pragma once
#include "cUnit.h"
class cBuilding : public cUnit
{
public:
	enum eBuildingType
	{
		E_ORDERNEXUS,
		E_ORDERINHIBITOR,
		E_ORDERTURRET,
		E_CHAOSNEXUS,
		E_CHAOSINHIBITOR,
		E_CHAOSTURRET,
	};

private:
	bool			m_bSelect;
	float			m_fSpeed;
	SYNTHESIZE(eBuildingType, m_eType, Type);
public:
	cBuilding();
	~cBuilding();

	virtual void Update() override;

	void KeyControl();
	void SetSelect(bool select) { m_bSelect = select; }
};

