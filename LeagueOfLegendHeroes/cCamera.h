#pragma once

#define g_pCamera cCamera::GetInstance()

#define CAMERA_MIN_DISTANCE 2
#define CAMERA_MOVE_ANGLE_SPD 0.005
#define CAMERA_ZOOM_SPD 10.0f

class cCamera
{
private:
	SINGLETON(cCamera);
	
private:
	D3DXVECTOR3		m_vCameraPos;

	D3DXVECTOR3*	m_pvTarget;

	float			m_fDistanceFromTarget;
	D3DXVECTOR3		m_vRotation;
	D3DXVECTOR3		m_vRelativePos;

	D3DXMATRIXA16	m_mRotateX;
	D3DXMATRIXA16	m_mRotateY;

	D3DXMATRIXA16	m_mLookAtLH;
	D3DXMATRIXA16	m_mPerspectiveFovLH;

	POINT			m_PrevMousePoint;
	bool			m_RButtonClicked;

public:
	void Destroy();
	void Update();
	void SetTarget(D3DXVECTOR3* target) { m_pvTarget = target; g_pShaderManager->SetTarget(target); }
	D3DXVECTOR3* GetTarget() { return m_pvTarget; }

	void Zoom(float spd);

	void KeyControl();
	D3DXMATRIX GetLookAtLH() { return m_mLookAtLH; }
	void SetLookAtLH(D3DXMATRIX mat) { m_mLookAtLH = mat; }

	D3DXVECTOR3 GetPos() { return m_vCameraPos; }
	D3DXVECTOR3 GetDir();
	D3DXVECTOR3 GetDirParrallelToPlane();
};

