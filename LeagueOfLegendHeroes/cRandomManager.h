#pragma once

#define g_pRND	cRandomManager::GetInstance()

class cRandomManager
{
	SINGLETON(cRandomManager)
public:
		
	int GetInt(int num);

	//fromNum ���� toNum���� ����
	int GetFromIntTo(int fromNum, int toNum);

	float GetFloat();
	float GetFloat(float num);
	float GetFromFloatTo(float fromFloat, float toFloat);

	//ũ�Ⱑ fromFloat~toFloat �� ���ҵ��� ���� ���͸� ����
	D3DXVECTOR3 GetFromVectorTo(float fromFloat, float toFloat);

	void Destroy();
};

