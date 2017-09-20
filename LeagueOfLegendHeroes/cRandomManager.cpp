#include "stdafx.h"
#include "cRandomManager.h"

#include <time.h>

cRandomManager::cRandomManager()
{
	srand((unsigned int)GetTickCount());
}

cRandomManager::~cRandomManager()
{
}

int cRandomManager::GetInt(int num)
{
	return rand() % num;
}

int cRandomManager::GetFromIntTo(int fromNum, int toNum)
{
	return rand() % (toNum - fromNum) + fromNum;
}

float cRandomManager::GetFloat()
{
	return ((float)rand() / (float)RAND_MAX);
}

float cRandomManager::GetFloat(float num)
{
	return GetFloat() *num;
}

float cRandomManager::GetFromFloatTo(float fromFloat, float toFloat)
{
	return GetFloat()*(toFloat-fromFloat)+ fromFloat;
}

D3DXVECTOR3 cRandomManager::GetFromVectorTo(float fromFloat, float toFloat)
{
	return D3DXVECTOR3(GetFromFloatTo(fromFloat, toFloat), GetFromFloatTo(fromFloat, toFloat), GetFromFloatTo(fromFloat, toFloat));
}
