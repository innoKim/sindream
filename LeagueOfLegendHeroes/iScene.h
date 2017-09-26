#pragma once

class iScene
{
public:
	iScene();
	virtual ~iScene();

	virtual void Setup()=0;
	virtual void Update()=0;
	virtual void Render()=0;

	virtual void UIRender() = 0;

	virtual D3DXVECTOR3 playerPos() = 0;
};

