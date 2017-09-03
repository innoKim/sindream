#include "stdafx.h"
#include "timer.h"
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

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

void timer::tick(float lockFPS)
{
	if (m_bIsHardware)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&m_nCurTime);
	}
	else
	{
		m_nCurTime = timeGetTime();
	}

	m_fTimeElapsed = (m_nCurTime - m_nLastTime)*m_fTimeScale;

	if (lockFPS > 0.0f)
	{
		while (m_fTimeElapsed < (1.0f / lockFPS))
		{
			if (m_bIsHardware)
			{
				QueryPerformanceCounter((LARGE_INTEGER*)&m_nCurTime);
			}
			else
			{
				m_nCurTime = timeGetTime();
			}
			m_fTimeElapsed = (m_nCurTime - m_nLastTime)*m_fTimeScale;
		}
	}

	m_nLastTime = m_nCurTime;
	m_dFPSFrameCount++;
	m_fFPSTimeElapsed += m_fTimeElapsed;
	m_fWorldTime += m_fTimeElapsed;

	if (m_fFPSTimeElapsed > 1.0f)
	{
		m_dFrameRate = m_dFPSFrameCount;
		m_dFPSFrameCount = 0;
		m_fFPSTimeElapsed = 0.0f;
	}
}

unsigned long timer::GetFrameRate(char* str)const
{
	if (str != NULL) sprintf(str, "FPS : %d", m_dFrameRate);
	return m_dFrameRate;
}