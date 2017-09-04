#pragma once

class iMap
{
public:
	iMap();
	virtual ~iMap();

	virtual void Setup() = 0;
	virtual void Render() = 0;
	virtual float GetHeight(D3DXVECTOR3 vPos) = 0;
};