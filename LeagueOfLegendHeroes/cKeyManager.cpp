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

//�ѹ��� ��������..
bool cKeyManager::IsOnceKeyDown(int key)
{
	//GetKeyState : �����͸� �˻� ����
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

//�ѹ��� ��������..
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

//��� ������ ������..
bool cKeyManager::IsStayKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000) return true;
	return false;
}

//���� on ���� off ~~~~~ (���� ����ġ ����...)
bool cKeyManager::IsToggleKey(int key)
{
	if (GetKeyState(key) & 0x0001) return true;
	return false;
}
