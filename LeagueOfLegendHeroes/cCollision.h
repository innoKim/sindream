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
	
	//�÷��� ������Ʈ�� ���������϶��� �ڿ��� �о��ִ³� ,�ٸ������϶��� ������� �������� �ȴ�.
	void FindVectorCollided(OUT D3DXVECTOR3& vPlus, OUT D3DXVECTOR3& vMinus, IN D3DXVECTOR3 norm, IN cPhysics& objPlus, IN cPhysics& objMinus);
	
public:
	void Destroy();

	void ObjVSObj(cPhysics& obj1, cPhysics& obj2);		//������Ʈ���� �浹
	void ObjVSObstacle(cPhysics& obj);					//�������� �浹 �ϴ� �ٴڸ� �������� �������� �� üũ�Ҽ� ������ �߰�
};

