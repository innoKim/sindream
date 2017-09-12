#pragma once
#include "cGroup.h"

class cOBJLoader
{
private:
	map<string, cMtlTex*>	m_mapMtl;

	vector<ST_PC_VERTEX>	m_vecSur;

	float minX, maxX, minZ, maxZ;

public:
	cOBJLoader();
	~cOBJLoader();

	void Load(IN char* Folder, char* FilePath, OUT vector<cGroup*> &vecGroup);
	vector<ST_PC_VERTEX> LoadSur(char* Filepath);
protected:
	void LoadMtl(char* Folder, char* FilePath);
};