#pragma once

#define g_pRND	cRandomManager::GetInstance()

class cRandomManager
{
	SINGLETON(cRandomManager)
public:
		
	int GetInt(int num);

	//fromNum 부터 toNum까지 랜덤
	int GetFromIntTo(int fromNum, int toNum);

	float GetFloat();
	float GetFloat(float num);
	float GetFromFloatTo(float fromFloat, float toFloat);

	//크기가 fromFloat~toFloat 인 원소들을 갖는 벡터를 생성
	D3DXVECTOR3 GetFromVectorTo(float fromFloat, float toFloat);

	void Destroy();
};

