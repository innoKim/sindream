#pragma once
class cPlane
{
private:
	LPD3DXMESH m_pMesh;
	D3DMATERIAL9 m_material;
	vector<D3DXVECTOR3> m_vecVertex;
	
public:
	cPlane();
	virtual ~cPlane();
	
	void Setup(int size);
	void Render();
};

