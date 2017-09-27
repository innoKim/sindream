#include "stdafx.h"
#include "cTimeManager.h"

void cTimeManager::Setup()
{
	m_pTimer = new timer;
	m_pTimer->Setup();
}

void cTimeManager::Destroy()
{
	if (m_pTimer)
	{
		m_pTimer->Release();
		delete m_pTimer;
	}
}

void cTimeManager::Update(float lock)
{
	if (m_pTimer != NULL) m_pTimer->tick(lock);


}

cTimeManager::cTimeManager()
{
	Setup();
}

cTimeManager::~cTimeManager()
{
}
