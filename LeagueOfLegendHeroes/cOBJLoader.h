#pragma once
#include "cGroup.h"

class cOBJLoader
{
private:
	map<string, cMtlTex*>	m_mapMtl;

	float minX, maxX, minZ, maxZ;

public:
	cOBJLoader();
	~cOBJLoader();

	void Load(IN char* Folder, char* FilePath, OUT vector<cGroup*> &vecGroup);
	void LoadSur(IN char* Filepath, OUT vector<ST_PC_VERTEX> &vecSur, vector<ST_PC_VERTEX> &vecGrid);
protected:
	void LoadMtl(char* Folder, char* FilePath);
};