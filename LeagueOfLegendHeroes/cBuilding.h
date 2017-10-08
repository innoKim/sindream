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
	float			m_fSpeed;
	SYNTHESIZE(bool, m_bSelect, Select);
	SYNTHESIZE(eBuildingType, m_eType, Type);
public:
	cBuilding();
	~cBuilding();

	virtual void Setup(vector<ST_UNITLOADINFO> statesVector, cMap* mapPtr) override;
	virtual void Update() override;

	void KeyControl();
};

