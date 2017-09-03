#include "stdafx.h"
#include "timer.h"
#include <mmsystem.h>

#pragma commetn(lib, "winmm.lib")

timer::timer()
{
}

timer::~timer()
{
}

void timer::Setup()
{
	if (QueryPerformanceFrequency((LARGE_INTEGER*)&m_nPeriodFrequency))
	{
		m_bIsHardware = true;
		QueryPerformanceCounter((LARGE_INTEGER*)&m_nLastTime);

		m_fTimeScale = 1.0f / m_nPeriodFrequency;
	}
	else
	{
		m_bIsHardware = false;

		m_nLastTime = timeGetTime();
		m_fTimeScale = 0.001f;
	}

	m_dFrameRate = 0;
	m_dFPSFrameCount = 0;
	m_fFPSTimeElapsed = 0.0f;
	m_fWorldTime = 0.0f;
}

void timer::Release()
{
}
