#pragma once

#define g_pPhysicsManager cPhysicsManager::GetInstance()

class cPhysics;
class cRigidbody;
class cRigidbodyCollision;

class cPhysicsManager
{
	SINGLETON(cPhysicsManager)
private:
	set<cPhysics*> m_setPhysicsObj;
	set<cPhysics*>::iterator m_itorPhysicsObj;

	set<cRigidbody*> m_setRigidbody;
	set<cRigidbody*>::iterator m_itorRigidbody;

	void ClearOverlapped(cRigidbody* obj1, cRigidbody* obj2, cRigidbodyCollision* collision);

public:
	void Destroy();

	void AddToSet(cPhysics*);
	void RemoveFromSet(cPhysics*);

	void AddRigidbodyToSet(cRigidbody*);
	void RemoveRigidbodyFromSet(cRigidbody*);
	
	void RigidbodyUpdate();

	set<cPhysics*>* GetSet() { return &m_setPhysicsObj; }
	set<cRigidbody*>* GetRigidbodySet() { return &m_setRigidbody; }

	set<cPhysics*> GetTargets(D3DXVECTOR3 posision, float radius);
};

