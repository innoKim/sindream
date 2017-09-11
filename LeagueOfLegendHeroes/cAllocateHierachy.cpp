#include "stdafx.h"
#include "cAllocateHierachy.h"
#include "cMtlTex.h"

cAllocateHierachy::cAllocateHierachy()
{
}

cAllocateHierachy::~cAllocateHierachy()
{
}

STDMETHODIMP cAllocateHierachy::CreateFrame(LPCSTR Name, LPD3DXFRAME * ppNewFrame)
{
	D3DXFRAME_CUSTOM* frame = new D3DXFRAME_CUSTOM;
	ZeroMemory(frame, sizeof(D3DXFRAME_CUSTOM));

	D3DXMatrixIdentity(&frame->TransformationMatrix);
	D3DXMatrixIdentity(&frame->CombinedTransformationMatrix);

	if (Name)
	{
		frame->Name = new char[strlen(Name) + 1];
		strcpy(frame->Name, Name);
	}
	*ppNewFrame = frame;

	return S_OK;
}

STDMETHODIMP cAllocateHierachy::CreateMeshContainer(
	LPCSTR Name,
	CONST D3DXMESHDATA * pMeshData,
	CONST D3DXMATERIAL * pMaterials,
	CONST D3DXEFFECTINSTANCE * pEffectInstances,
	DWORD NumMaterials,
	CONST DWORD * pAdjacency,
	LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER * ppNewMeshContainer)
{
	D3DXMESHCONTAINER_CUSTOM* container = new D3DXMESHCONTAINER_CUSTOM;

	ZeroMemory(container, sizeof(D3DXMESHCONTAINER_CUSTOM));

	for (DWORD i = 0; i < NumMaterials; ++i)
	{
		cMtlTex* mtlTex = new cMtlTex;
		std::string sFile(pMaterials[i].pTextureFilename);
		string sPath = string("./Zealot/") + string(pMaterials[i].pTextureFilename);
		mtlTex->SetTexture(g_pTextureManager->GetTexture(sPath));
		mtlTex->SetMtl(pMaterials[i].MatD3D);
		container->vecTM.push_back(mtlTex);
	}

	if (pMeshData && pMeshData->Type &&pMeshData->Type == D3DXMESHTYPE_MESH)
	{
		container->MeshData.Type = pMeshData->Type;
		container->MeshData.pMesh = pMeshData->pMesh;
		pMeshData->pMesh->AddRef();

		pMeshData->pMesh->CloneMeshFVF(D3DXMESH_MANAGED,
			pMeshData->pMesh->GetFVF(),
			g_pD3DDevice,
			&container->originMesh);
		pMeshData->pMesh->CloneMeshFVF(D3DXMESH_MANAGED,
			pMeshData->pMesh->GetFVF(),
			g_pD3DDevice,
			&container->workMesh);
	}

	if (pSkinInfo)
	{
		container->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();



		container->workMesh->GetAttributeTable(NULL, &container->NumAttributeGroups);
		container->attributeTable = new D3DXATTRIBUTERANGE[container->NumAttributeGroups];
		container->workMesh->GetAttributeTable(container->attributeTable, NULL);

		int NumBones = pSkinInfo->GetNumBones();

		container->pBoneOffsetMatrixes = new D3DXMATRIX[NumBones];
		container->pCurrentBoneMatrixes = new D3DXMATRIX[NumBones];

		for (int i = 0; i < NumBones; i++)
		{
			container->pBoneOffsetMatrixes[i]
				= *(container->pSkinInfo->GetBoneOffsetMatrix(i));
		}
	}

	(*ppNewMeshContainer) = container;

	return S_OK;
}

STDMETHODIMP cAllocateHierachy::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	D3DXFRAME_CUSTOM* frame = (D3DXFRAME_CUSTOM*)pFrameToFree;
	if (frame->Name)
	{
		SAFE_DELETE_ARRAY(frame->Name);
	}
	delete pFrameToFree;

	return E_NOTIMPL;
}

STDMETHODIMP cAllocateHierachy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	D3DXMESHCONTAINER_CUSTOM* container = (D3DXMESHCONTAINER_CUSTOM*)pMeshContainerToFree;
	SAFE_RELEASE(container->pSkinInfo);
	SAFE_RELEASE(container->MeshData.pMesh);
	SAFE_RELEASE(container->originMesh);
	SAFE_RELEASE(container->workMesh);

	SAFE_DELETE_ARRAY(container->ppBoneMatrixPtrs);
	SAFE_DELETE_ARRAY(container->pBoneOffsetMatrixes);
	SAFE_DELETE_ARRAY(container->pCurrentBoneMatrixes);


	for (int i = 0; i < container->vecTM.size(); i++)
	{
		SAFE_DELETE(container->vecTM[i]);
	}

	delete container;
	return S_OK;
}