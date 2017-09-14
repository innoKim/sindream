#pragma once

class cPhysics;

class cCollision
{
private:
	void FindCollidedVelocity(OUT float& v1, OUT float& v2, IN cPhysics& obj1, IN cPhysics& obj2);
	float FindMomentumCharge(float v, cPhysics& obj);
	D3DXVECTOR3 FindNormalToCollisionPlane(cPhysics& obj1, cPhysics& obj2);
	
	//�÷��� ������Ʈ�� ���������϶��� �ڿ��� �о��ִ³� ,�ٸ������϶��� ������� �������� �ȴ�.
	void FindVectorCollided(OUT D3DXVECTOR3& vPlus, OUT D3DXVECTOR3& vMinus, IN D3DXVECTOR3 norm, IN cPhysics& objPlus, IN cPhysics& objMinus);
	
public:
	cCollision();
	virtual ~cCollision();
	
	void ObjVSObj(cPhysics& obj1, cPhysics& obj2);		//������Ʈ���� �浹
	void ObjVSObstacle();								//�������� �浹
};

