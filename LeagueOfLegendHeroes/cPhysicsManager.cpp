#include "stdafx.h"
#include "cPhysicsManager.h"
#include "cRigidbodyCollision.h"
#include "cRigidbody.h"

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

void cPhysicsManager::AddRigidbodyToSet(cRigidbody * newRigidbody)
{
	m_setRigidbody.insert(newRigidbody);
}

void cPhysicsManager::RemoveRigidbodyFromSet(cRigidbody *Rigidbody)
{
	m_itorRigidbody = m_setRigidbody.find(Rigidbody);

	if (m_itorRigidbody != m_setRigidbody.end())
	{
		m_setRigidbody.erase(m_itorRigidbody);
	}
}

void cPhysicsManager::RigidbodyUpdate()
{
	static bool forceApplied = false;

	for each (auto p in m_setRigidbody)
	{
		//지형과의 충돌처리
		cRigidbody obstacle;
		obstacle.SetRadius(1.0f);
		D3DXVECTOR3 obsPos = *p->GetPositionPtr();
		obstacle.SetPositionPtr(&D3DXVECTOR3(obsPos.x, 0, obsPos.z));
		obstacle.SetMass(OBSTACLE_MASS);

		cRigidbodyCollision col(p, &obstacle);
		eCollisionState colStat = col.FindCollisionState();

		switch (colStat)
		{
		case COLLISION_NONE:
			break;
		case COLLISION_CONTACT:
			col.CalculateReaction();
			forceApplied = false;
			(*p->GetPositionPtr()).y = 0.0f;
			break;
		case COLLISION_OVERLAPPED:
			ClearOverlapped(p, &obstacle, &col);
			(*p->GetPositionPtr()).y = 0.0f;
			forceApplied = false;
			break;
		default:
			break;
		}


		//오브젝트간의 충돌처리
		if (p->IsCollidedAtThisLoop()) continue;

		for each (auto q in m_setRigidbody)
		{
			if (q->IsCollidedAtThisLoop()) continue;
			if (p == q) continue;			
			
			cRigidbodyCollision col(p, q);
			eCollisionState colStat = col.FindCollisionState();

			switch (colStat)
			{
			case COLLISION_NONE:
				break;
			case COLLISION_CONTACT:
				col.CalculateReaction();
				forceApplied = false;
				p->ResetRestDuration();
				q->ResetRestDuration();
				break;
			case COLLISION_OVERLAPPED:
				ClearOverlapped(p,q,&col);
				p->ResetRestDuration();
				q->ResetRestDuration();
				forceApplied = false;
				break;
			default:
				break;
			}		
		}
	}

	if (forceApplied)
	{
		for each (auto p in m_setRigidbody)
		{
			p->GetForce().Reset();
		}
	}
	else
	{
		forceApplied = true;
	}

	for each (auto p in m_setRigidbody)
	{
		p->Update(g_pTimeManager->GetElapsedTime());
	}
}

void cPhysicsManager::ClearOverlapped(cRigidbody * obj1, cRigidbody * obj2, cRigidbodyCollision * collision)
{
	eCollisionState colStat = COLLISION_OVERLAPPED;
	float ElapsedTime = g_pTimeManager->GetElapsedTime();

	for (bool done = false; (!done);)
	{
		switch (colStat)
		{
			case COLLISION_NONE:
				obj1->Update(ElapsedTime);
				obj2->Update(ElapsedTime);
				break;

			case COLLISION_CONTACT:
				collision->CalculateReaction();
				done = true;
				break;

			case COLLISION_OVERLAPPED:	
				obj1->SetAngularVelocity(-obj1->GetAngularVelocity());
				obj1->SetVelocity(-obj1->GetVelocity());
				obj1->GetForce().force *= -1.0f;

				obj2->SetAngularVelocity(-obj2->GetAngularVelocity());
				obj2->SetVelocity(-obj2->GetVelocity());
				obj2->GetForce().force *= -1.0f;

				obj1->Update(ElapsedTime);
				obj1->Update(ElapsedTime);

				ElapsedTime *= 0.5f;

				obj1->SetAngularVelocity(-obj1->GetAngularVelocity());
				obj1->SetVelocity(-obj1->GetVelocity());
				obj1->GetForce().force *= -1.0f;

				obj2->SetAngularVelocity(-obj2->GetAngularVelocity());
				obj2->SetVelocity(-obj2->GetVelocity());
				obj2->GetForce().force *= -1.0f;

				obj1->Update(ElapsedTime);
				obj1->Update(ElapsedTime);

				colStat = collision->FindCollisionState();
				break;

			default:
				break;
		}
	}
}

