#include "stdafx.h"
#include "cOBJLoader.h"

cOBJLoader::cOBJLoader()
{
}

cOBJLoader::~cOBJLoader()
{
}

void cOBJLoader::Load(IN char* Folder, char* FilePath, OUT vector<cGroup*>& vecGroup, OUT map<string, cMtlTex*>& mapMtlTex)
{
	float minX = 0.0f, maxX = 0.0f;
	float minY = 0.0f, maxY = 0.0f;
	float minZ = 0.0f, maxZ = 0.0f;

	vector<D3DXVECTOR3>		vecP;
	vector<D3DXVECTOR2>		vecT;
	vector<ST_PT_VERTEX>	vecVertex;

	char szMtl[1024] = { '\0', };

	string szFolder = Folder;
	string szFile = FilePath;

	string szFullPath = szFolder + szFile;

	FILE* fp = 0;
	fopen_s(&fp, szFullPath.c_str(), "r");

	while (!feof(fp))
	{
		char szBuf[1024] = { '\0', };
		fgets(szBuf, 1024, fp);

		if (szBuf[0] <= 33) continue;

		else if (szBuf[0] == 'm')
		{
			char szPath[1024] = { '\0', };
			sscanf_s(szBuf, "%*s %s", szPath, 1024);
			LoadMtl(Folder, szPath, mapMtlTex);
		}

		else if (szBuf[0] == 'g')
		{
			if (!vecVertex.empty())
			{
				cGroup* pGroup = new cGroup;
				pGroup->SetVertex(vecVertex);
				vecGroup.push_back(pGroup);
				vecVertex.clear();
				pGroup->SetMtlTex(mapMtlTex[szMtl]);
				D3DXMATRIXA16 matW;
				D3DXMatrixRotationX(&matW, -D3DX_PI / 2.0f);
				pGroup->SetWorld(matW);
			}
		}

		else if (szBuf[0] == 'v')
		{
			float x, y, z;
			if (szBuf[1] == ' ')
			{
				sscanf_s(szBuf, "%*s %f %f %f", &x, &y, &z, 1024);
				vecP.push_back(D3DXVECTOR3(x * MAP_RATIO, y * MAP_RATIO, z * MAP_RATIO));
			}
			else if (szBuf[1] == 't')
			{
				sscanf_s(szBuf, "%*s %f %f %*f", &x, &y, 1024);
				vecT.push_back(D3DXVECTOR2(x, 1 - y));
			}
		}
		else if (szBuf[0] == 'u')
		{
			sscanf_s(szBuf, "%*s %s", szMtl, 1024);
		}
		else if (szBuf[0] == 'f')
		{
			int nIndex[3][2];
			sscanf_s(szBuf, "%*s %d/%d %d/%d %d/%d", &nIndex[0][0], &nIndex[0][1], &nIndex[1][0], &nIndex[1][1], &nIndex[2][0], &nIndex[2][1], 1024);

			for (size_t i = 0; i < 3; i++)
			{
				D3DXVECTOR3 p = vecP[nIndex[i][0] - 1];
				D3DXVECTOR2 t = vecT[nIndex[i][1] - 1];
				vecVertex.push_back(ST_PT_VERTEX(p, t));

				if (p.x > maxX) maxX = p.x;	
				if (p.x < minX) minX = p.x;	

				if (p.y > maxY) maxY = p.y;	
				if (p.y < minY) minY = p.y;	

				if (p.z > maxZ) maxZ = p.z;	
				if (p.z < minZ) minZ = p.z;	
			}
		}
	}

	fclose(fp);
}

void cOBJLoader::LoadSur(IN char * Filepath, OUT vector<ST_PC_VERTEX> &vecSur, vector<vector<ST_HEIGHTNODE>>& gridHeightNode)
{

	float minX = 0.0f, maxX = 0.0f;
	float minY = 0.0f, maxY = 0.0f;
	float minZ = 0.0f, maxZ = 0.0f;
	
	gridHeightNode.clear();
	gridHeightNode.resize(MAP_GRID);

	for (int i = 0; i < MAP_GRID; i++)
	{
		gridHeightNode[i].resize(MAP_GRID);
	}

	for (int x = 0; x < gridHeightNode.size(); x++)
	{
		for (int z = 0; z < gridHeightNode[x].size(); z++)
		{
			gridHeightNode[x][z].xIndex = x;
			gridHeightNode[x][z].zIndex = z;

			float width = MAP_SIZE*MAP_RATIO / (float)MAP_GRID;

			gridHeightNode[x][z].sq = ST_SQUARE(x*width, z*width, (x + 1)*width, (z + 1)*width);
		}
	}
		
	FILE* fp = 0;
	fopen_s(&fp, Filepath, "r");

	while (!feof(fp))
	{
		char szBuf[1024] = { '\0', };
		fgets(szBuf, 1024, fp);

		if (szBuf[0] <= 33) continue;

		else if (szBuf[0] == 'O') continue;

		else if (szBuf[0] == 'T') continue;

		else if (szBuf[0] == 't')
		{
			D3DXVECTOR3 a;
			D3DXVECTOR3 b;
			D3DXVECTOR3 c;

			fgets(szBuf, 1024, fp);
			sscanf_s(szBuf, "%f %f %f %*f %*f", &a.x, &a.y, &a.z);
			fgets(szBuf, 1024, fp);
			sscanf_s(szBuf, "%f %f %f %*f %*f", &b.x, &b.y, &b.z);
			fgets(szBuf, 1024, fp);
			sscanf_s(szBuf, "%f %f %f %*f %*f", &c.x, &c.y, &c.z);

			a = a*MAP_RATIO;
			b = b*MAP_RATIO;
			c = c*MAP_RATIO;

			a.z = -a.z;
			b.z = -b.z;
			c.z = -c.z;

			vecSur.push_back(ST_PC_VERTEX(a, D3DCOLOR_XRGB(255, 255, 255)));
			vecSur.push_back(ST_PC_VERTEX(b, D3DCOLOR_XRGB(255, 255, 255)));
			vecSur.push_back(ST_PC_VERTEX(c, D3DCOLOR_XRGB(255, 255, 255)));

			SortTriangleToGrid(gridHeightNode, a, b, c);
		}
	}
	fclose(fp);
}
//
//void cOBJLoader::CreateGrid()
//{
//	// 15000
//
//	for (int j = 0; j < GRID; j++)
//	{
//		for (int i = 0; i < GRID; i++)
//		{
//			m_stGrid[j * GRID + i].vecLT = D3DXVECTOR3(i * (15000 / GRID) * RATIO, -72.f * RATIO, ((15000 / GRID) + j * (15000 / GRID)) * RATIO);
//			m_stGrid[j * GRID + i].vecRT = D3DXVECTOR3(((15000 / GRID) + i * (15000 / GRID)) * RATIO, -72.f * RATIO, ((15000 / GRID) + j * (15000 / GRID)) * RATIO);
//			m_stGrid[j * GRID + i].vecLB = D3DXVECTOR3(i * (15000 / GRID) * RATIO, -72.f * RATIO, j * (15000 / GRID) * RATIO);
//			m_stGrid[j * GRID + i].vecRB = D3DXVECTOR3(((15000 / GRID) + i * (15000 / GRID)) * RATIO, -72.f * RATIO, j * (15000 / GRID) * RATIO);
//		}
//	}
//}
//
//void cOBJLoader::DivideVertex(stGrid &Grid, vector<ST_PC_VERTEX> vecSur)
//{
//	// 삼각형에서 그리드
//	for (int i = 0; i < vecSur.size() - 2; i += 3)
//	{
//		D3DXVECTOR3 p0 = vecSur[i + 0].p;
//		D3DXVECTOR3 p1 = vecSur[i + 1].p;
//		D3DXVECTOR3 p2 = vecSur[i + 2].p;
//
//		float fDist = 300.f;
//
//		float fToGrid = fDist;
//
//		D3DXVECTOR3 Dir = D3DXVECTOR3(0, -1, 0);
//
//		if (D3DXIntersectTri(&Grid.vecLT, &Grid.vecRT, &Grid.vecLB, &p0, &Dir, 0, 0, &fDist))
//		{
//			if (fToGrid > fDist)
//			{
//				Grid.vecVertex.push_back(vecSur[i + 0].p);
//				Grid.vecVertex.push_back(vecSur[i + 1].p);
//				Grid.vecVertex.push_back(vecSur[i + 2].p);
//			}
//
//		}
//		else if (D3DXIntersectTri(&Grid.vecRB, &Grid.vecRT, &Grid.vecLB, &p0, &Dir, 0, 0, &fDist))
//		{
//			if (fToGrid > fDist)
//			{
//				Grid.vecVertex.push_back(vecSur[i + 0].p);
//				Grid.vecVertex.push_back(vecSur[i + 1].p);
//				Grid.vecVertex.push_back(vecSur[i + 2].p);
//			}
//		}
//		else if (D3DXIntersectTri(&Grid.vecLT, &Grid.vecRT, &Grid.vecLB, &p1, &Dir, 0, 0, &fDist))
//		{
//			if (fToGrid > fDist)
//			{
//				Grid.vecVertex.push_back(vecSur[i + 0].p);
//				Grid.vecVertex.push_back(vecSur[i + 1].p);
//				Grid.vecVertex.push_back(vecSur[i + 2].p);
//			}
//		}
//		else if (D3DXIntersectTri(&Grid.vecRB, &Grid.vecRT, &Grid.vecLB, &p1, &Dir, 0, 0, &fDist))
//		{
//			if (fToGrid > fDist)
//			{
//				Grid.vecVertex.push_back(vecSur[i + 0].p);
//				Grid.vecVertex.push_back(vecSur[i + 1].p);
//				Grid.vecVertex.push_back(vecSur[i + 2].p);
//			}
//		}
//		else if (D3DXIntersectTri(&Grid.vecLT, &Grid.vecRT, &Grid.vecLB, &p2, &Dir, 0, 0, &fDist))
//		{
//			if (fToGrid > fDist)
//			{
//				Grid.vecVertex.push_back(vecSur[i + 0].p);
//				Grid.vecVertex.push_back(vecSur[i + 1].p);
//				Grid.vecVertex.push_back(vecSur[i + 2].p);
//			}
//		}
//		else if (D3DXIntersectTri(&Grid.vecRB, &Grid.vecRT, &Grid.vecLB, &p2, &Dir, 0, 0, &fDist))
//		{
//			if (fToGrid > fDist)
//			{
//				Grid.vecVertex.push_back(vecSur[i + 0].p);
//				Grid.vecVertex.push_back(vecSur[i + 1].p);
//				Grid.vecVertex.push_back(vecSur[i + 2].p);
//			}
//		}
//	}
//
//}

void cOBJLoader::LoadMtl(IN char* Folder, IN char * FilePath, OUT map<string, cMtlTex*>& mapMtlTex)
{
	string szFolder = Folder;
	string szFile = FilePath;

	string szFullPath = szFolder + szFile;

	FILE* fp = 0;
	fopen_s(&fp, szFullPath.c_str(), "r");

	char szMtlName[1024] = { '\0', };

	while (!feof(fp))
	{
		char szBuf[1024] = { '\0', };
		fgets(szBuf, 1024, fp);

		if (szBuf[0] < 33) continue;

		else if (szBuf[0] == 'n')
		{
			sscanf_s(szBuf, "%*s %s", szMtlName, 1024);

			mapMtlTex[szMtlName] = new cMtlTex;
		}

		else if (szBuf[0] == 'K')
		{
			D3DMATERIAL9& stMtl = mapMtlTex[szMtlName]->GetMtl();

			float r, g, b;

			if (szBuf[1] == 'a')
			{
				sscanf_s(szBuf, "%*s %f %f %f", &r, &g, &b, 1024);
				stMtl.Ambient = D3DXCOLOR(r, g, b, 1.f);
			}
			else if (szBuf[1] == 'd')
			{
				sscanf_s(szBuf, "%*s %f %f %f", &r, &g, &b, 1024);
				stMtl.Diffuse = D3DXCOLOR(r, g, b, 1.f);
			}
			else if (szBuf[1] == 's')
			{
				sscanf_s(szBuf, "%*s %f %f %f", &r, &g, &b, 1024);
				stMtl.Specular = D3DXCOLOR(r, g, b, 1.f);
			}
		}
		else if (szBuf[0] == 'm')
		{
			char szTexPath[1024] = { '\0', };
			sscanf_s(szBuf, "%*s %s", szTexPath, 1024);
			string szFolder = Folder;
			string szFileName(szTexPath);
			string szFullPath = Folder + szFileName;
			LPDIRECT3DTEXTURE9 pTex = g_pTextureManager->GetTexture(szFullPath.c_str());
			mapMtlTex[szMtlName]->SetTexture(pTex);
		}
	}

	fclose(fp);
}

void cOBJLoader::SortTriangleToGrid(vector<vector<ST_HEIGHTNODE>>& gridHeightNode, D3DXVECTOR3& a, D3DXVECTOR3& b, D3DXVECTOR3& c)
{
	D3DXVECTOR3 point[3];
	point[0] = a;
	point[1] = b;
	point[2] = c;

	float gridWidth = MAP_SIZE*MAP_RATIO / (float)MAP_GRID;

	//삼각형과 마주칠 가능성이 있는 노드의 인덱스범위를 구한다.
	int minX = 0, minZ = 0 , maxX = 0, maxZ = 0;
	
	for (int i = 0; i < 3; i++)
	{
		int indexX = point[i].x / gridWidth;
		int indexZ = point[i].z / gridWidth;
		if (i == 0)
		{
			minX = maxX = indexX;
			minZ = maxZ = indexZ;
		}
		else
		{
			if (minX > indexX) minX = indexX;
			if (maxX < indexX) maxX = indexX;
			if (minZ > indexZ) minZ = indexZ;
			if (maxZ < indexZ) maxZ = indexZ;
		}
	}

	//대상노드들과 삼각형을 충돌시켜보고 충돌되면 버텍스들을 추가한다.
	ST_TRIANGLE tri(D3DXVECTOR2(a.x, a.z), D3DXVECTOR2(b.x, b.z), D3DXVECTOR2(c.x, c.z));
	
	for (int x = minX; x <= maxX; x++)
	{
		for (int z = minZ; z <= maxZ; z++)
		{
			if (TriVsSq(tri, gridHeightNode[x][z].sq))
			{
				gridHeightNode[x][z].vecVertex.push_back(a);
				gridHeightNode[x][z].vecVertex.push_back(b);
				gridHeightNode[x][z].vecVertex.push_back(c);
			}
		}
	}
}
