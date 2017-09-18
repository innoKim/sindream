#pragma once
#include "cGroup.h"

#define RATIO	0.5f

class cOBJLoader
{
private:
	float minX, maxX, minZ, maxZ;

public:
	cOBJLoader();
	~cOBJLoader();

	void Load(IN char* Folder, char* FilePath, OUT vector<cGroup*> &vecGroup, OUT map<string, cMtlTex*>& mapMtlTex);
	void LoadSur(IN char* Filepath, OUT vector<ST_PC_VERTEX> &vecSur, vector<ST_PC_VERTEX> &vecGrid);
protected:
	void LoadMtl(IN char* Folder, IN char* FilePath, OUT map<string, cMtlTex*>& mapMtlTex);
};