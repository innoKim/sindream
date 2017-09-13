#pragma once

#include "cAllocateHierachy.h"

class cSkinnedMesh
{
private:
	cAllocateHierachy						m_alloc;

	LPD3DXFRAME_CUSTOM			m_pRoot;

	LPD3DXANIMATIONCONTROLLER	m_pAnimController;
	LPD3DXANIMATIONSET			m_pAnimSet;

	CALLBACKFUNC				m_pfnCallBack;
	void*						m_pCallBackObj;

public:
	cSkinnedMesh();
	~cSkinnedMesh();

	void Setup(const char* filePath);
	void SetCallBack(CALLBACKFUNC CallBackFunction, void* Obj);

	void Update();
	void Render(D3DXVECTOR3* Pos, D3DXVECTOR3* Dir);

private:
	void SetupBoneMatrixPtrs(D3DXFRAME * pFrame);

	void Update(LPD3DXFRAME pFrame, LPD3DXFRAME pParent);
	void UpdateSkinnedMesh(D3DXFRAME * pFrame);

	void CircularRender(D3DXFRAME * target, D3DXMATRIX worldMatrix);
};

