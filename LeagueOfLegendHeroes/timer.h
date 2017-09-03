#pragma once
class timer
{
private:
	bool			m_bIsHardware;
	float			m_fTimeElapsed;
	float			m_fTimeScale;

	__int64			m_nCurTime;
	__int64			m_nLastTime;
	__int64			m_nPeriodFrequency;

	unsigned long	m_dFrameRate;
	unsigned long	m_dFPSFrameCount;

	float			m_fFPSTimeElapsed;
	float			m_fWorldTime;

public:
	void Setup();
	void Release();
	void tick(float lockFPS = 0.0f);

	unsigned long GetFrameRate(char * str = (char*)0) const;

	inline float	GetElapsedTime(void) const { return m_fTimeElapsed; }
	inline float	GetWorldTime(void) const { return m_fWorldTime; }
	
	timer();
	~timer();
};

