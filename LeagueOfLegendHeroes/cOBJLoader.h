#pragma once
#include "cGroup.h"

class cOBJLoader
{
private:
	map<string, cMtlTex*> m_mapMtl;

public:
	cOBJLoader();
	~cOBJLoader();

	void Load(IN char* Folder, char* FilePath, OUT vector<cGroup*> &vecGroup);
protected:
	void LoadMtl(char* Folder, char* FilePath);
};