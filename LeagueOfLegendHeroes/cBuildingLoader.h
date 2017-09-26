#pragma once

class cBuilding;
class cMap;

class cBuildingLoader
{
public:
	cBuildingLoader();
	~cBuildingLoader();

	void LoadBuilding(IN char* szFileName, IN cMap* pMap, OUT vector<cBuilding*>& vecBuilding);
	void SaveBuilding(IN char* szFileName, IN vector<cBuilding*>& vecBuilding);
};

