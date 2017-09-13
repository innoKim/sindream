#include "stdafx.h"
#include "cSkinnedMesh.h"
#include "cMtlTex.h"

cSkinnedMesh::cSkinnedMesh():
	m_pRoot(NULL),
	m_pAnimController(NULL),
	m_pfnCallBack(NULL),
	m_pCallBackObj(NULL)
{
}


cSkinnedMesh::~cSkinnedMesh()
{
	D3DXFrameDestroy(m_pRoot, &m_alloc);
}

void cSkinnedMesh::Setup(const char * filePath)
{
	LPD3DXANIMATIONCONTROLLER* m_ppAnimController = new LPD3DXANIMATIONCONTROLLER;
	D3DXLoadMeshHierarchyFromX(filePath, D3DXMESH_MANAGED, g_pD3DDevice, &m_alloc, NULL, (LPD3DXFRAME*)&m_pRoot, &m_pAnimController);

	SetupBoneMatrixPtrs(m_pRoot);
}

void cSkinnedMesh::SetCallBack(CALLBACKFUNC CallBackFunction, void * Obj)
{
	m_pfnCallBack = CallBackFunction;
	m_pCallBackObj = Obj;
}

void cSkinnedMesh::Update()
{
	m_pAnimController->AdvanceTime(g_pTimeManager->GetElapsedTime(), NULL);

	if (m_pRoot)
	{
		Update(m_pRoot, NULL);
		UpdateSkinnedMesh(m_pRoot);
	}

	m_pAnimController->GetTrackAnimationSet(0, &m_pAnimSet);

	if (m_pAnimController->GetTime()>m_pAnimSet->GetPeriod() && m_pfnCallBack)
	{
		m_pfnCallBack(m_pCallBackObj);
		m_pAnimController->ResetTime();
	}
}

void cSkinnedMesh::Render(D3DXVECTOR3 * Pos, D3DXVECTOR3 * Dir)
{
	D3DXMATRIX matS, matT, matR, worldMat;
	D3DXMatrixScaling(&matS, 0.01f, 0.01f, 0.01f);
	D3DXMatrixTranslation(&matT, Pos->x, Pos->y, Pos->z);

	D3DXMatrixLookAtLH(&matR, &D3DXVECTOR3(0, 0, 0), &(-(*Dir)), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixTranspose(&matR, &matR);

	worldMat = matS*matR*matT;

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	CircularRender(m_pRoot, worldMat);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
}

void cSkinnedMesh::SetupBoneMatrixPtrs(D3DXFRAME * pFrame)
{
	if (pFrame->pMeshContainer)
	{
		D3DXMESHCONTAINER_CUSTOM* container = (D3DXMESHCONTAINER_CUSTOM*)pFrame->pMeshContainer;

		if (container->pSkinInfo)
		{
			int NumBones = container->pSkinInfo->GetNumBones();
			container->ppBoneMatrixPtrs = new D3DXMATRIX*[NumBones];

			for (int i = 0; i < NumBones; i++)
			{
				D3DXFRAME_CUSTOM* frame = (D3DXFRAME_CUSTOM*)D3DXFrameFind(
					m_pRoot, container->pSkinInfo->GetBoneName(i));
				if (frame)
				{
					container->ppBoneMatrixPtrs[i] = &frame->CombinedTransformationMatrix;
				}
				else
				{
					container->ppBoneMatrixPtrs[i] = NULL;
				}
			}
		}
	}

	if (pFrame->pFrameSibling)
	{
		SetupBoneMatrixPtrs(pFrame->pFrameSibling);
	}
	if (pFrame->pFrameFirstChild)
	{
		SetupBoneMatrixPtrs(pFrame->pFrameFirstChild);
	}
}

void cSkinnedMesh::Update(LPD3DXFRAME pFrame, LPD3DXFRAME pParent)
{
	if (pFrame)
	{
		D3DXFRAME_CUSTOM* pBone = (D3DXFRAME_CUSTOM*)pFrame;
		pBone->CombinedTransformationMatrix = pBone->TransformationMatrix;
		if (pParent)
		{
			D3DXFRAME_CUSTOM* pParentBone = (D3DXFRAME_CUSTOM*)pParent;
			pBone->CombinedTransformationMatrix =
				pBone->TransformationMatrix *
				pParentBone->CombinedTransformationMatrix;
		}
	}

	if (pFrame->pFrameFirstChild)
	{
		Update(pFrame->pFrameFirstChild, pFrame);
	}

	if (pFrame->pFrameSibling)
	{
		Update(pFrame->pFrameSibling, pParent);
	}
}

void cSkinnedMesh::UpdateSkinnedMesh(D3DXFRAME * pFrame)
{
	D3DXFRAME_CUSTOM* frame = (D3DXFRAME_CUSTOM*)pFrame;
	D3DXMESHCONTAINER_CUSTOM * container = (D3DXMESHCONTAINER_CUSTOM *)frame->pMeshContainer;

	if (container)
	{
		int a = 0;
		if (container->pSkinInfo)
		{
			int numBones = container->pSkinInfo->GetNumBones();
			for (int i = 0; i < numBones; i++)
			{
				D3DXMatrixMultiply(&container->pCurrentBoneMatrixes[i],
					&container->pBoneOffsetMatrixes[i],
					container->ppBoneMatrixPtrs[i]);
			}
			BYTE *src = NULL, *dest = NULL;
			container->originMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&src);
			container->workMesh->LockVertexBuffer(0, (void**)&dest);
			container->pSkinInfo->UpdateSkinnedMesh(container->pCurrentBoneMatrixes, NULL, src, dest);
			container->workMesh->UnlockVertexBuffer();
			container->originMesh->UnlockVertexBuffer();
		}
	}

	if (pFrame->pFrameSibling)
	{
		UpdateSkinnedMesh(pFrame->pFrameSibling);
	}
	if (pFrame->pFrameFirstChild)
	{
		UpdateSkinnedMesh(pFrame->pFrameFirstChild);
	}
}

void cSkinnedMesh::CircularRender(D3DXFRAME * target, D3DXMATRIX worldMatrix)
{
	D3DXFRAME_CUSTOM* frame = (D3DXFRAME_CUSTOM*)target;

	if (frame->pMeshContainer)
	{
		D3DXMESHCONTAINER_CUSTOM* container = (D3DXMESHCONTAINER_CUSTOM*)frame->pMeshContainer;
		for (int i = 0; i<container->vecTM.size(); i++)
		{
			g_pD3DDevice->SetTransform(D3DTS_WORLD, &worldMatrix);
			g_pD3DDevice->SetTexture(0, container->vecTM[i]->GetTexture());
			g_pD3DDevice->SetMaterial(&container->vecTM[i]->GetMtl());
			container->workMesh->DrawSubset(i);
		}
	}

	if (frame->pFrameSibling)
	{
		CircularRender(frame->pFrameSibling, worldMatrix);
	}

	if (frame->pFrameFirstChild)
	{
		CircularRender(frame->pFrameFirstChild, worldMatrix);
	}
}
