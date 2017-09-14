#pragma once

#define g_pCollisionCalculator cCollision::GetInstance()

#define EPSILON_VELOCITY 1.0f

class cPhysics;

class cCollision
{
	SINGLETON(cCollision);

private:
	void FindCollidedVelocity(OUT float& v1, OUT float& v2, IN cPhysics& obj1, IN cPhysics& obj2);
	float FindMomentumCharge(float v, cPhysics& obj);
	D3DXVECTOR3 FindNormalToCollisionPlane(cPhysics& obj1, cPhysics& obj2);
	
	//플러스 오브젝트는 같은방향일때는 뒤에서 밀어주는놈 ,다른방향일때는 모멘텀이 작은놈이 된다.
	void FindVectorCollided(OUT D3DXVECTOR3& vPlus, OUT D3DXVECTOR3& vMinus, IN D3DXVECTOR3 norm, IN cPhysics& objPlus, IN cPhysics& objMinus);
	
public:
	void Destroy();

	void ObjVSObj(cPhysics& obj1, cPhysics& obj2);		//오브젝트간의 충돌
	void ObjVSObstacle(cPhysics& obj);					//지형과의 충돌 일단 바닥만 만들었어요 나머지는 벽 체크할수 있으면 추가
};

