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

	//ÇÑ¹ø¸¸ ´­·¶À»¶§..
	bool IsOnceKeyDown(int key);

	//ÇÑ¹ø¸¸ ¶¼¾úÀ»¶§..
	bool IsOnceKeyUp(int key);

	//°è¼Ó ´©¸£°í ÀÖÀ»¶§..
	bool IsStayKeyDown(int key);

	//µþ±ï on µþ±ï off ~~~~~ (Àüµî ½ºÀ§Ä¡ °³³ä...)
	bool IsToggleKey(int key);

	bitset<KEYMAX> GetKeyUp() { return _keyUp; }
	bitset<KEYMAX> GetKeyDown() { return _keyDown; }

	void SetKeyDown(int key, bool state) { _keyDown.set(key, state); }
	void SetKeyUp(int key, bool state) { _keyUp.set(key, state); }
};

