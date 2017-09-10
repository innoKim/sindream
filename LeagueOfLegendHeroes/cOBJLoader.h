#pragma once

class cOBJLoader
{
private:
	ST_PC_VERTEX			stVertex;
	vector<ST_PC_VERTEX>	vecVertex;
public:
	cOBJLoader();
	~cOBJLoader();

	vector<ST_PC_VERTEX> Load(char* FileName);
};