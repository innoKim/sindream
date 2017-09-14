#pragma once

#include "iScene.h"
#include "cOBJLoader.h"


class cTestScene : public iScene
{
	vector<cGroup*>			m_vecMap;
	vector<ST_PC_VERTEX>	m_vecSur;
	vector<ST_PC_VERTEX>	m_vecGrid;

	D3DXMATRIX		m_matW, m_matS, m_matR, m_matT;

	// �׽�Ʈ��
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

