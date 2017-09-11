#pragma once


struct D3DXFRAME_CUSTOM :public D3DXFRAME
{
	D3DXMATRIX CombinedTransformationMatrix;
};

typedef D3DXFRAME_CUSTOM* LPD3DXFRAME_CUSTOM;

class cMtlTex;
struct D3DXMESHCONTAINER_CUSTOM : public D3DXMESHCONTAINER
{
	std::vector<cMtlTex*>	vecTM;

	LPD3DXMESH originMesh;
	LPD3DXMESH workMesh;

	DWORD			NumAttributeGroups;
	D3DXATTRIBUTERANGE* attributeTable;

	D3DXMATRIX**	ppBoneMatrixPtrs;
	D3DXMATRIX*		pBoneOffsetMatrixes;
	D3DXMATRIX*		pCurrentBoneMatrixes;
};

class cAllocateHierachy : public ID3DXAllocateHierarchy
{
public:
	cAllocateHierachy();
	~cAllocateHierachy();

	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name,
		LPD3DXFRAME *ppNewFrame) override;

	STDMETHOD(CreateMeshContainer)(THIS_
		LPCSTR Name,
		CONST D3DXMESHDATA *pMeshData,
		CONST D3DXMATERIAL *pMaterials,
		CONST D3DXEFFECTINSTANCE *pEffectInstances,
		DWORD NumMaterials,
		CONST DWORD *pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER *ppNewMeshContainer) override;

	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree) override;
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree) override;
};

