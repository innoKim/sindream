#include "stdafx.h"
#include "cBuildingLoader.h"
#include "cBuilding.h"


cBuildingLoader::cBuildingLoader()
{
}


cBuildingLoader::~cBuildingLoader()
{
}

void cBuildingLoader::LoadBuilding(IN cMap* pMap, OUT vector<cBuilding*>& vecBuilding)
{
	for each(auto p in vecBuilding)
	{
		SAFE_DELETE(p);
	}
	vecBuilding.clear();

	ifstream inFile("unit/buildings.txt", ios::in);

	vector<string> vecStrMap;

	while (!inFile.eof())
	{
		string str;
		inFile >> str;
		vecStrMap.push_back(str);
	}

	for (int i = 1; i < vecStrMap.size(); i += 7)
	{
		if (vecStrMap[i] == "ORDERNEXUS")
		{
			cBuilding* pOrderNexus = new cBuilding;
			vector<ST_UNITLOADINFO> tempOrderNexus;
			tempOrderNexus.push_back({ STATE_IDLE, "unit/OrderNexus.x", NULL, NULL });
			pOrderNexus->Setup(tempOrderNexus, pMap);
			pOrderNexus->SetPosition(D3DXVECTOR3(atof(vecStrMap[i + 1].c_str()), atof(vecStrMap[i + 2].c_str()), atof(vecStrMap[i + 3].c_str())));
			pOrderNexus->SetDirection(D3DXVECTOR3(atof(vecStrMap[i + 4].c_str()), atof(vecStrMap[i + 5].c_str()), atof(vecStrMap[i + 6].c_str())));
			pOrderNexus->SetType(cBuilding::E_ORDERNEXUS);
			vecBuilding.push_back(pOrderNexus);
		}
		else if (vecStrMap[i] == "ORDERINHIBITOR")
		{
			cBuilding* pOrderInhibitor = new cBuilding;
			vector<ST_UNITLOADINFO> tempOrderInhibitor;
			tempOrderInhibitor.push_back({ STATE_IDLE, "unit/OrderInhibitor.x", NULL, NULL });
			pOrderInhibitor->Setup(tempOrderInhibitor, pMap);
			pOrderInhibitor->SetPosition(D3DXVECTOR3(atof(vecStrMap[i + 1].c_str()), atof(vecStrMap[i + 2].c_str()), atof(vecStrMap[i + 3].c_str())));
			pOrderInhibitor->SetDirection(D3DXVECTOR3(atof(vecStrMap[i + 4].c_str()), atof(vecStrMap[i + 5].c_str()), atof(vecStrMap[i + 6].c_str())));
			pOrderInhibitor->SetType(cBuilding::E_ORDERINHIBITOR);
			vecBuilding.push_back(pOrderInhibitor);
		}
		else if (vecStrMap[i] == "ORDERTURRET")
		{
			cBuilding* pOrderTurret = new cBuilding;
			vector<ST_UNITLOADINFO> tempOrderTurret;
			tempOrderTurret.push_back({ STATE_IDLE, "unit/OrderTurret.x", NULL, NULL });
			pOrderTurret->Setup(tempOrderTurret, pMap);
			pOrderTurret->SetPosition(D3DXVECTOR3(atof(vecStrMap[i + 1].c_str()), atof(vecStrMap[i + 2].c_str()), atof(vecStrMap[i + 3].c_str())));
			pOrderTurret->SetDirection(D3DXVECTOR3(atof(vecStrMap[i + 4].c_str()), atof(vecStrMap[i + 5].c_str()), atof(vecStrMap[i + 6].c_str())));
			pOrderTurret->SetType(cBuilding::E_ORDERTURRET);
			vecBuilding.push_back(pOrderTurret);
		}
		else if (vecStrMap[i] == "CHAOSNEXUS")
		{
			cBuilding* pChaosNexus = new cBuilding;
			vector<ST_UNITLOADINFO> tempChaosNexus;
			tempChaosNexus.push_back({ STATE_IDLE, "unit/ChaosNexus.x", NULL, NULL });
			pChaosNexus->Setup(tempChaosNexus, pMap);
			pChaosNexus->SetPosition(D3DXVECTOR3(atof(vecStrMap[i + 1].c_str()), atof(vecStrMap[i + 2].c_str()), atof(vecStrMap[i + 3].c_str())));
			pChaosNexus->SetDirection(D3DXVECTOR3(atof(vecStrMap[i + 4].c_str()), atof(vecStrMap[i + 5].c_str()), atof(vecStrMap[i + 6].c_str())));
			pChaosNexus->SetType(cBuilding::E_CHAOSNEXUS);
			vecBuilding.push_back(pChaosNexus);
		}
		else if (vecStrMap[i] == "CHAOSINHIBITOR")
		{
			cBuilding* pChaosInhibitor = new cBuilding;
			vector<ST_UNITLOADINFO> tempChaosInhibitor;
			tempChaosInhibitor.push_back({ STATE_IDLE, "unit/ChaosInhibitor.x", NULL, NULL });
			pChaosInhibitor->Setup(tempChaosInhibitor, pMap);
			pChaosInhibitor->SetPosition(D3DXVECTOR3(atof(vecStrMap[i + 1].c_str()), atof(vecStrMap[i + 2].c_str()), atof(vecStrMap[i + 3].c_str())));
			pChaosInhibitor->SetDirection(D3DXVECTOR3(atof(vecStrMap[i + 4].c_str()), atof(vecStrMap[i + 5].c_str()), atof(vecStrMap[i + 6].c_str())));
			pChaosInhibitor->SetType(cBuilding::E_CHAOSINHIBITOR);
			vecBuilding.push_back(pChaosInhibitor);
		}
		else if (vecStrMap[i] == "CHAOSTURRET")
		{
			cBuilding* pChaosTurret = new cBuilding;
			vector<ST_UNITLOADINFO> tempChaosTurret;
			tempChaosTurret.push_back({ STATE_IDLE, "unit/ChaosTurret.x", NULL, NULL });
			pChaosTurret->Setup(tempChaosTurret, pMap);
			pChaosTurret->SetPosition(D3DXVECTOR3(atof(vecStrMap[i + 1].c_str()), atof(vecStrMap[i + 2].c_str()), atof(vecStrMap[i + 3].c_str())));
			pChaosTurret->SetDirection(D3DXVECTOR3(atof(vecStrMap[i + 4].c_str()), atof(vecStrMap[i + 5].c_str()), atof(vecStrMap[i + 6].c_str())));
			pChaosTurret->SetType(cBuilding::E_CHAOSTURRET);
			vecBuilding.push_back(pChaosTurret);
		}
	}

	inFile.close();
}

void cBuildingLoader::SaveBuilding(IN vector<cBuilding*>& vecBuilding)
{
	HANDLE file;
	DWORD write;

	string strData;

	strData += to_string(vecBuilding.size()) + '\n';

	for (int i = 0; i < vecBuilding.size(); i++)
	{
		switch (vecBuilding[i]->GetType())
		{
		case cBuilding::E_ORDERNEXUS:
			strData += "ORDERNEXUS ";
			break;
		case cBuilding::E_ORDERINHIBITOR:
			strData += "ORDERINHIBITOR ";
			break;
		case cBuilding::E_ORDERTURRET:
			strData += "ORDERTURRET ";
			break;
		case cBuilding::E_CHAOSNEXUS:
			strData += "CHAOSNEXUS ";
			break;
		case cBuilding::E_CHAOSINHIBITOR:
			strData += "CHAOSINHIBITOR ";
			break;
		case cBuilding::E_CHAOSTURRET:
			strData += "CHAOSTURRET ";
			break;
		}

		strData += to_string(vecBuilding[i]->GetPosition().x) + " ";
		strData += to_string(vecBuilding[i]->GetPosition().y) + " ";
		strData += to_string(vecBuilding[i]->GetPosition().z) + " ";

		strData += to_string(vecBuilding[i]->GetDirection().x) + " ";
		strData += to_string(vecBuilding[i]->GetDirection().y) + " ";
		strData += to_string(vecBuilding[i]->GetDirection().z) + " ";

		strData += '\n';
	}

	file = CreateFile("unit/buildings.txt", GENERIC_WRITE, 0,
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(file, strData.c_str(), strData.length(), &write, NULL);
	CloseHandle(file);
}
