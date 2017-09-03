#pragma once

#define g_pKeyManager cKeyManager::GetInstance()

#include <bitset>

#define KEYMAX 256

class cKeyManager
{
private:
	bitset<KEYMAX> _keyUp;
	bitset<KEYMAX> _keyDown;

private:
	SINGLETON(cKeyManager);

public:
	HRESULT Setup(void);
	void Destroy(void);

	//�ѹ��� ��������..
	bool IsOnceKeyDown(int key);

	//�ѹ��� ��������..
	bool IsOnceKeyUp(int key);

	//��� ������ ������..
	bool IsStayKeyDown(int key);

	//���� on ���� off ~~~~~ (���� ����ġ ����...)
	bool IsToggleKey(int key);

	bitset<KEYMAX> GetKeyUp() { return _keyUp; }
	bitset<KEYMAX> GetKeyDown() { return _keyDown; }

	void SetKeyDown(int key, bool state) { _keyDown.set(key, state); }
	void SetKeyUp(int key, bool state) { _keyUp.set(key, state); }
};

