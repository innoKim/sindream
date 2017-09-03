#pragma once

#include "timer.h"
#define g_pTimeManager cTimeManager::GetInstance()

class cTimeManager
{
	SINGLETON(cTimeManager);
private:
	timer* m_pTimer;
public:
	void Setup();
	void Destroy();
	void Update(float lock);

	inline float GetWorldTime() const { return m_pTimer->GetWorldTime(); }
	inline float GetElapsedTime() const { return m_pTimer->GetElapsedTime(); }
	inline float GetFrameRate() const { return m_pTimer->GetFrameRate(); }
};