#pragma once

#include "iScene.h"
#include "cOBJLoader.h"

class cShader;

class cTestScene : public iScene
{
	cShader*		m_pShader;

	vector<cGroup*>	m_vecGroup;

	D3DXMATRIX		m_matW, m_matS, m_matR, m_matT;

	vector<ST_PC_VERTEX> m_vecSur;

	// 테스트용
	D3DXVECTOR3		m_vPos;
	D3DXVECTOR3		m_vDir;
	float			m_fAngle;
	float			m_fSpeed;

public:
	cTestScene();
	virtual ~cTestScene();

	virtual void Setup() override;
	virtual void Update() override;
	virtual void Render() override;
};

