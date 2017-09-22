#pragma once

#include "cAllocateHierachy.h"

struct ST_CallbackInfo
{
	float						fTime;
	CALLBACKFUNC				pfnCallBack;
	void*						pCallBackObj;
	
	ST_CallbackInfo(float time = 0.0f, CALLBACKFUNC cbfn = NULL, void* cbobj = NULL) { fTime = time, pfnCallBack = cbfn, pCallBackObj = cbobj; }
};

class cSkinnedMesh
{
private:
	cAllocateHierachy						m_alloc;

	LPD3DXFRAME_CUSTOM			m_pRoot;

	LPD3DXANIMATIONCONTROLLER	m_pAnimController;
	LPD3DXANIMATIONSET			m_pAnimSet;

	vector<ST_CallbackInfo>	m_vecCallbackInfo;
	
public:
	cSkinnedMesh();
	~cSkinnedMesh();

	void Setup(const char* filePath);
	void SetCallBack(CALLBACKFUNC CallBackFunction, void* Obj, float time);

	void Update();
	void Render(D3DXVECTOR3* Pos, D3DXVECTOR3* Dir);

private:
	void SetupBoneMatrixPtrs(D3DXFRAME * pFrame);

	void Update(LPD3DXFRAME pFrame, LPD3DXFRAME pParent);
	void UpdateSkinnedMesh(D3DXFRAME * pFrame);

	void CircularRender(D3DXFRAME * target, D3DXMATRIX worldMatrix);
};

