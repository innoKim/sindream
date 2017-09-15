#include "stdafx.h"
#include "cOBJLoader.h"

cOBJLoader::cOBJLoader()
			: minX(500.f)
			, maxX(500.f)
			, minZ(500.f)
			, maxZ(500.f)
{
}

cOBJLoader::~cOBJLoader()
{
}

void cOBJLoader::Load(IN char* Folder, char* FilePath, OUT vector<cGroup*>& vecGroup)
{
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
			LoadMtl(Folder, szPath);
		}

		else if (szBuf[0] == 'g')
		{
			if (!vecVertex.empty())
			{
				LPD3DXMESH pMesh;
				D3DXCreateMeshFVF(vecVertex.size() / 3, vecVertex.size(), D3DXMESH_MANAGED, ST_PT_VERTEX::FVF, g_pD3DDevice, &pMesh);

				ST_PT_VERTEX* vb = NULL;
				pMesh->LockVertexBuffer(0, (LPVOID*)&vb);

				for (int i = 0; i < vecVertex.size(); i++)
				{
					vb[i] = vecVertex[i];
				}

				pMesh->UnlockVertexBuffer();

				WORD* ib = NULL;
				pMesh->LockIndexBuffer(0, (LPVOID*)&ib);

				for (int i = 0; i < vecVertex.size(); i++)
				{
					ib[i] = i;
				}

				pMesh->UnlockIndexBuffer();

				vector<DWORD> adjagencyInfo(pMesh->GetNumFaces() * 3);
				pMesh->GenerateAdjacency(0.0f, &adjagencyInfo[0]);

				pMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
					&adjagencyInfo[0], NULL, 0, 0);

				cGroup* pGroup = new cGroup;
				//pGroup->SetVertex(vecVertex);
				vecGroup.push_back(pGroup);
				vecVertex.clear();
				pGroup->SetMesh(pMesh);
				pGroup->SetMtlTex(m_mapMtl[szMtl]);
				D3DXMATRIXA16 matW;
				D3DXMatrixRotationX(&matW, -D3DX_PI / 2.0f);
				pGroup->SetWorld(matW);

				SAFE_RELEASE(pMesh);
			}
		}

		else if (szBuf[0] == 'v')
		{
			float x, y, z;
			if (szBuf[1] == ' ')
			{
				sscanf_s(szBuf, "%*s %f %f %f", &x, &y, &z, 1024);
				vecP.push_back(D3DXVECTOR3(x * RATIO, y * RATIO, z * RATIO));
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
			}
		}
	}

	for each(auto p in m_mapMtl)
	{
		SAFE_RELEASE(p.second);
	}

	fclose(fp);
}

void cOBJLoader::LoadSur(IN char * Filepath, OUT vector<ST_PC_VERTEX> &vecSur, vector<ST_PC_VERTEX> &vecGrid)
{
	FILE* fp = 0;
	fopen_s(&fp, Filepath, "r");
	
	while (!feof(fp))
	{
		char szBuf[1024] = { '\0', };
		fgets(szBuf, 1024, fp);

		if (szBuf[0] <= 33) continue;

		else if (szBuf[0] == 'O') continue;

		else if (szBuf[0] == 'T') continue;

		else if (szBuf[0] == 't') continue;

		else
		{
			float x, y, z;
			sscanf_s(szBuf, "%f %f %f %*f %*f", &x, &y, &z);

			D3DXVECTOR3 p = D3DXVECTOR3(x * RATIO, y * RATIO, -z * RATIO);
			D3DXCOLOR	c = D3DCOLOR_XRGB(255, 255, 255);
			vecSur.push_back(ST_PC_VERTEX(p, c));

			if (x > maxX) maxX = x;		 // 14618.4004
			if (x < minX) minX = x;		 // 98.8951035  14521
			if (-z > maxZ) maxZ = -z;	 // 14692.2109
			if (-z < minZ) minZ = -z;	 // 132.755798  14561

		}
	}

	for (int i = 0; i <= 5; i++)
	{
		D3DXCOLOR c = D3DCOLOR_XRGB(255, 255, 255);

		vecGrid.push_back(ST_PC_VERTEX(D3DXVECTOR3(0, 0, i * 2940), c));
		vecGrid.push_back(ST_PC_VERTEX(D3DXVECTOR3(14700, 0, i * 2940), c));

		vecGrid.push_back(ST_PC_VERTEX(D3DXVECTOR3(i * 2940, 0, 0), c));
		vecGrid.push_back(ST_PC_VERTEX(D3DXVECTOR3(i * 2940, 0, 14700), c));
	}

	fclose(fp);
}

void cOBJLoader::LoadMtl(char* Folder, char * FilePath)
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

			m_mapMtl[szMtlName] = new cMtlTex;
		}

		else if (szBuf[0] == 'K')
		{
			D3DMATERIAL9& stMtl = m_mapMtl[szMtlName]->GetMtl();

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
			m_mapMtl[szMtlName]->SetTexture(pTex);
		}
	}

	fclose(fp);
}