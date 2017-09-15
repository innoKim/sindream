#include "stdafx.h"
#include "cRigidbodyCollision.h"
#include "cRigidbody.h"

cRigidbodyCollision::cRigidbodyCollision()
{
}


cRigidbodyCollision::cRigidbodyCollision(cRigidbody * object1, cRigidbody * object2)
{
}

cRigidbodyCollision::~cRigidbodyCollision()
{
}

eCollisionState cRigidbodyCollision::CollisionOccurred()
{
	eCollisionState result = COLLISION_NONE;

	//두 구체 사이의 틈 거리 (중점의 거리가 아닌 표면들 사이의 최소거리)
	float dist = fabs(D3DXVec3Length(&((*m_pObj1->GetPositionPtr()) - (*m_pObj1->GetPositionPtr()))) - m_pObj1->GetRadius() - m_pObj1->GetRadius());
	
	if (dist < EPSILON_COLLISION)
	{
		result = COLLISION_CONTACT;
	}
	else if (dist < 0.0f)
	{
		result = COLLISION_OVERLAPPED;
	}
	
	return result;
}

bool cRigidbodyCollision::CalculateReaction()
{
	float e = (m_pObj1->GetElastic() + m_pObj2->GetElastic()) / 2;
	
	//상대적인 각운동량
	D3DXVECTOR3 vr = m_pObj1->GetAngularVelocity() - m_pObj1->GetAngularVelocity();

	//둘 사이의 접촉면 노말
	D3DXVECTOR3 n;
	D3DXVec3Normalize(&n, &(*m_pObj1->GetPositionPtr() - *m_pObj2->GetPositionPtr()));

	//반지름 방향의 벡터
	D3DXVECTOR3 r1 = (-n)*m_pObj1->GetRadius();
	D3DXVECTOR3 r2 = n*m_pObj2->GetRadius();

	float rm1 = 1.0f / m_pObj1->GetMass();
	float rm2 = 1.0f / m_pObj2->GetMass();

	// 접하는 순간 각운동량으로인해 결정되는 순간 힘은
	// f = (-vr(e+1)) /[rm1+rm2+n(dot)[(r1(cross)n/I1)(cross)r1]+n(dot)[(r2(cross)n/I2)(cross)r2]]
	
	// up		:	(-vr(e+1))
	// down1	:	n(dot)[(r1(cross)n/I1)(cross)r1]
	// down2	:	n(dot)[(r2(cross)n/I2)(cross)r2]
	// f = up/[rm1+rm2+down1+down2] 로 연산을 할것

	// up		:	(-vr(e+1))
	D3DXVECTOR3 up = -vr*(e + 1.0f);

	// down1	:	n(dot)[(r1(cross)n/I1)(cross)r1]
	D3DXVECTOR3 r1_c_n;
	D3DXVec3Cross(&r1_c_n, &r1, &n);
	r1_c_n.x = r1_c_n.x / m_pObj1->GetRotationInertia().x;
	r1_c_n.y = r1_c_n.y / m_pObj1->GetRotationInertia().y;
	r1_c_n.z = r1_c_n.z / m_pObj1->GetRotationInertia().z;

	D3DXVECTOR3 _c_r1;
	D3DXVec3Cross(&_c_r1, &r1_c_n, &r1);

	float down1 = D3DXVec3Dot(&n, &_c_r1);

	// down2	:	n(dot)[(r2(cross)n/I2)(cross)r2]
	D3DXVECTOR3 r2_c_n;
	D3DXVec3Cross(&r2_c_n, &r2, &n);
	r2_c_n.x = r2_c_n.x / m_pObj2->GetRotationInertia().x;
	r2_c_n.y = r2_c_n.y / m_pObj2->GetRotationInertia().y;
	r2_c_n.z = r2_c_n.z / m_pObj2->GetRotationInertia().z;

	D3DXVECTOR3 _c_r2;
	D3DXVec3Cross(&_c_r2, &r2_c_n, &r2);

	float down2 = D3DXVec3Dot(&n, &_c_r2);

	// f = up/[rm1+rm2+down1+down2] 로 연산을 할것

	//충돌로 생긴 힘교환량
	D3DXVECTOR3 f = up / (rm1 + rm2 + down1 + down2);

	//최종적인 힘
	m_pObj1->GetForce().force += f;
	m_pObj2->GetForce().force -= f;

	return true;
}
