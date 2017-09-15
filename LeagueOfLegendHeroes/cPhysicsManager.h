#pragma once

#define g_pPhysicsManager cPhysicsManager::GetInstance()

class cPhysics;

class cPhysicsManager
{
	SINGLETON(cPhysicsManager)
private:
	set<cPhysics*> m_setPhysicsObj;
	set<cPhysics*>::iterator m_itorPhysicsObj;

public:
	void Destroy();

	void AddToSet(cPhysics*);
	void RemoveFromSet(cPhysics*);
	
	set<cPhysics*>* GetSet() { return &m_setPhysicsObj; }
};

