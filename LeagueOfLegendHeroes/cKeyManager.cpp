#include "stdAfx.h"
#include "cKeyManager.h"

HRESULT cKeyManager::Setup(void)
{
	for (int i = 0; i < KEYMAX; i++)
	{
		this->GetKeyUp().set(i, false);
		this->GetKeyDown().set(i, false);
	}
	return S_OK;
}

void cKeyManager::Destroy(void)
{
}

//한번만 눌렀을때..
bool cKeyManager::IsOnceKeyDown(int key)
{
	//GetKeyState : 누른것만 검사 가능
	if (GetAsyncKeyState(key) & 0x8000)
	{
		if (!this->GetKeyDown()[key])
		{
			this->SetKeyDown(key, true);
			return true;
		}
	}
	else this->SetKeyDown(key, false);
	return false;
}

//한번만 떼었을때..
bool cKeyManager::IsOnceKeyUp(int key)
{
	if (GetAsyncKeyState(key) & 0x8000) this->SetKeyUp(key, true);
	else
	{
		if (this->GetKeyUp()[key])
		{
			this->SetKeyUp(key, false);
			return true;
		}
	}
	return false;
}

//계속 누르고 있을때..
bool cKeyManager::IsStayKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000) return true;
	return false;
}

//딸깍 on 딸깍 off ~~~~~ (전등 스위치 개념...)
bool cKeyManager::IsToggleKey(int key)
{
	if (GetKeyState(key) & 0x0001) return true;
	return false;
}
