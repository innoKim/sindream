#include "stdafx.h"
#include "cOBJLoader.h"

cOBJLoader::cOBJLoader()
{
}

cOBJLoader::~cOBJLoader()
{
}

vector<ST_PC_VERTEX> cOBJLoader::Load(char * FileName)
{
	FILE* fp = 0;
	fopen_s(&fp, FileName, "r");

	int nCount = 0;

	while (!feof(fp))
	{
		char strBuf[1024] = { '\0', };
		fgets(strBuf, 1024, fp);

		nCount++;

		if (strlen(strBuf) < 33) continue;

		if (strBuf[0] == 'O') continue;

		if (strBuf[0] == 'T') continue;

		if (strBuf[0] == 't') continue;

		if (strBuf[0] == 'e') continue;

		else
		{
			float x, y, z, u, v;
			sscanf_s(strBuf, "%f %f %f %f %f", &x, &y, &z, &u, &v);
			stVertex.p.x = x * 0.01f;
			stVertex.p.y = -y * 0.01f;
			stVertex.p.z = -z * 0.01f;
			stVertex.c = D3DCOLOR_XRGB(255, 255, 255);

			vecVertex.push_back(stVertex);
		}

	}

	fclose(fp);

	return vecVertex;
}
