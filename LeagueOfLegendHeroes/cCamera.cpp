#include "StdAfx.h"
#include "cCamera.h"

cCamera::cCamera() :m_pvTarget(NULL), m_fDistanceFromTarget(0.0f), m_vCameraPos(0, 0, 0), m_vRotation(0, 0, 0), m_vRelativePos(0, 0, 0)
{

}

cCamera::~cCamera()
{
}

void cCamera::Setup()
{
	m_fDistanceFromTarget = 5.0f;
	m_vRotation.x = 0.5f;
	D3DXMatrixIdentity(&m_mLookAtLH);
	D3DXMatrixIdentity(&m_mPerspectiveFovLH);
}

void cCamera::Destroy()
{
}

void cCamera::Update()
{
	KeyControl();

	m_vRelativePos.x = 0.0f;
	m_vRelativePos.y = 0.0f;
	m_vRelativePos.z = -m_fDistanceFromTarget;

	D3DXMatrixRotationX(&m_mRotateX, m_vRotation.x);
	D3DXMatrixRotationY(&m_mRotateY, m_vRotation.y);
	D3DXVec3TransformCoord(&m_vRelativePos, &m_vRelativePos, &(m_mRotateX*m_mRotateY));

	//타겟이 있으면 타겟을 보고
	if (m_pvTarget)
	{
		m_vCameraPos = *m_pvTarget + m_vRelativePos;
		D3DXMatrixLookAtLH(&m_mLookAtLH, &m_vCameraPos, m_pvTarget, &D3DXVECTOR3(0, 1, 0));
		g_pD3DDevice->SetTransform(D3DTS_VIEW, &m_mLookAtLH);
	}
	//없으면 원점을 바라봄
	else
	{
		m_vCameraPos = m_vRelativePos;
		D3DXMatrixLookAtLH(&m_mLookAtLH, &m_vCameraPos, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 1, 0));
		g_pD3DDevice->SetTransform(D3DTS_VIEW, &m_mLookAtLH);
	}
	//화면 크기가 바뀌어도 카메라가 업데이트시 계속 투영매트릭스 적용
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	D3DXMatrixPerspectiveFovLH(&m_mPerspectiveFovLH, D3DX_PI / 4.0f, rc.right / (float)rc.bottom, 1, 1000);
	g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_mPerspectiveFovLH);
}

void cCamera::Zoom(float spd)
{
	m_fDistanceFromTarget += spd;

	if (m_fDistanceFromTarget < (float)CAMERA_MIN_DISTANCE) m_fDistanceFromTarget = (float)CAMERA_MIN_DISTANCE;
}

void cCamera::KeyControl()
{
	if (g_pKeyManager->IsOnceKeyDown(VK_RBUTTON))
	{
		m_PrevMousePoint = g_ptMouse;
		m_RButtonClicked = true;
	}
	else if (g_pKeyManager->IsOnceKeyUp(VK_RBUTTON))
	{
		m_RButtonClicked = false;
	}
	else if (m_RButtonClicked && g_pKeyManager->IsStayKeyDown(VK_RBUTTON))
	{
		float virticalFactor = g_ptMouse.y - m_PrevMousePoint.y;
		float horizontalFactor = g_ptMouse.x - m_PrevMousePoint.x;

		m_vRotation.y += horizontalFactor*CAMERA_MOVE_ANGLE_SPD;

		if (m_vRotation.x + virticalFactor*(float)CAMERA_MOVE_ANGLE_SPD < D3DX_PI / 2.0f
			&&m_vRotation.x + virticalFactor*(float)CAMERA_MOVE_ANGLE_SPD > D3DX_PI / -2.0f)
			m_vRotation.x += virticalFactor*(float)CAMERA_MOVE_ANGLE_SPD;

		m_PrevMousePoint = g_ptMouse;
	}

	if (g_nMouseWheel>0)
	{
		Zoom(-CAMERA_ZOOM_SPD);
		g_nMouseWheel--;
	}
	if (g_nMouseWheel<0)
	{
		Zoom(CAMERA_ZOOM_SPD);
		g_nMouseWheel++;
	}
}
