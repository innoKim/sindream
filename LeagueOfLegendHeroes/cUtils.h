#pragma once
#include <cmath>

#define FLOAT_EQUAL(f1, f2) (fabs(f1 - f2) <= FLOAT_EPSILON)

struct RayInfo
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 dir;

	RayInfo() :pos(0, 0, 0), dir(0, 0, 0) {}
	RayInfo(D3DXVECTOR3 position, D3DXVECTOR3 direction) { pos = position, dir = direction; }
};

struct HitInfo
{
	D3DXVECTOR3 hitpos;
	float dist;

	HitInfo() :hitpos(0, 0, 0), dist(0) {}
	HitInfo(D3DXVECTOR3 hitPosition, float distance) { hitpos = hitPosition, dist = distance; }
};

struct ST_PC_VERTEX;

namespace MY_UTIL
{
	// �Ÿ� ���Ѵ�
	float GetDistance(float startX, float startY, float endX, float endY);

	// �� ������ ���� ���Ѵ�
	float GetAngle(float startX, float startY, float endX, float endY);

	RayInfo Ray(D3DXVECTOR3 start, D3DXVECTOR3 end);

	//ȭ������ ������ ������ǥ�迡���� directionVector
	RayInfo RayToScreenPoint(int screenPosX, int screenPosY);

	bool RayCast(IN RayInfo& ray, OUT HitInfo& hit, IN vector<D3DXVECTOR3>& target);

	bool RayCastPC(IN RayInfo& ray, OUT HitInfo& hit, IN vector<ST_PC_VERTEX>* target);

	DWORD FtoDw(float f);
}