#include "stdafx.h"
#include "cPhysicsManager.h"


cPhysicsManager::cPhysicsManager()
{
}


cPhysicsManager::~cPhysicsManager()
{
}

void cPhysicsManager::Destroy()
{
}

void cPhysicsManager::AddToSet(cPhysics * newPhysicsObj)
{
	m_setPhysicsObj.insert(newPhysicsObj);
}

void cPhysicsManager::RemoveFromSet(cPhysics * PhysicsObj)
{
	m_itorPhysicsObj = m_setPhysicsObj.find(PhysicsObj);

	if (m_itorPhysicsObj != m_setPhysicsObj.end())
	{
		m_setPhysicsObj.erase(m_itorPhysicsObj);
	}
}
