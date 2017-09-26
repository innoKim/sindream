#pragma once

class cBuilding;
class cMap;

class cBuildingLoader
{
public:
	cBuildingLoader();
	~cBuildingLoader();

	void LoadBuilding(IN cMap* pMap, OUT vector<cBuilding*>& vecBuilding);
	void SaveBuilding(IN vector<cBuilding*>& vecBuilding);
};

