#include "stdafx.h"
#include "cUtils.h"

namespace MY_UTIL
{
	float MY_UTIL::GetDistance(float startX, float startY, float endX, float endY)
	{
		float x = endX - startX;
		float y = endY - endY;
		return sqrt(x * x + y * y);
	}

	float MY_UTIL::GetAngle(float startX, float startY, float endX, float endY)

	{
		float x = endX - startX;
		float y = endY - startY;
		float d = sqrt(x * x + y * y);
		float angle = acos(x / d);

		if (y > 0) angle = 2 * D3DX_PI - angle;
		return angle;
	}

	RayInfo Ray(D3DXVECTOR3 start, D3DXVECTOR3 end)
	{
		D3DXVECTOR3 dir = end - start;
		D3DXVec3Normalize(&dir, &dir);
		return RayInfo(start, dir);
	}

	RayInfo RayToScreenPoint(int screenPosX, int screenPosY)
	{
		float fx = 0.0f, fy = 0.0f;

		D3DVIEWPORT9 vp;
		g_pD3DDevice->GetViewport(&vp);

		D3DXMATRIX proj;
		g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &proj);

		fx = ((2.0f*screenPosX) / vp.Width - 1) / proj._11;
		fy = ((-2.0f*screenPosY) / vp.Height + 1) / proj._22;

		D3DXMATRIX view;
		g_pD3DDevice->GetTransform(D3DTS_VIEW, &view);
		D3DXMatrixInverse(&view, 0, &view);

		D3DXVECTOR3 cameraPos(0, 0, 0);
		D3DXVec3TransformCoord(&cameraPos, &cameraPos, &view);

		D3DXVECTOR3 rayInVeiwSpace(fx, fy, 1.0f);
		D3DXVec3TransformNormal(&rayInVeiwSpace, &rayInVeiwSpace, &view);
		D3DXVec3Normalize(&rayInVeiwSpace, &rayInVeiwSpace);

		return RayInfo(cameraPos, rayInVeiwSpace);
	}

	bool RayCast(IN RayInfo& ray, OUT HitInfo& hit, IN vector<D3DXVECTOR3>& target)
	{
		if (target.size() % 3 != 0 || target.size() < 3) return false;

		float dist = 9999.0f;
		hit.dist = dist;
		bool result = false;

		for (int i = 0; i < target.size(); i += 3)
		{
			if (D3DXIntersectTri(&target[i], &target[i + 1], &target[i + 2], &ray.pos, &ray.dir, 0, 0, &dist))
			{
				if (hit.dist > dist)
				{
					hit.dist = dist;
					hit.hitpos = ray.pos + hit.dist*ray.dir;
				}
				result = true;
			}
		}
		return result;
	}

	bool RayCastPC(IN RayInfo& ray, OUT HitInfo& hit, IN vector<ST_PC_VERTEX>* target)
	{
		if (target->size() % 3 != 0 || target->size() < 3) return false;

		float dist = 9999.0f;
		hit.dist = dist;
		bool result = false;

		for (int i = 0; i < target->size(); i += 3)
		{
			if (D3DXIntersectTri(&(*target)[i].p, &(*target)[i + 1].p, &(*target)[i + 2].p, &ray.pos, &ray.dir, 0, 0, &dist))
			{

				hit.dist = dist;
				hit.hitpos = ray.pos + hit.dist*ray.dir;
				if (hit.dist > dist)
				{
					
				}
				result = true;
			}
		}
		return result;
	}

	DWORD FtoDw(float f)
	{
		return *((DWORD*)&f);
	}
	D3DXVECTOR3 WorldToScreenPosition(D3DXVECTOR3 worldPosition)
	{
		D3DXVECTOR3 result = worldPosition;
		
		D3DXMATRIX view;
		g_pD3DDevice->GetTransform(D3DTS_VIEW, &view);

		D3DXMATRIX projection;
		g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &projection);

		D3DVIEWPORT9 vp;
		g_pD3DDevice->GetViewport(&vp);

		D3DXMATRIX viewport;
		D3DXMatrixIdentity(&viewport);
		
		float a = vp.Width;
		viewport._11 = a / 2.0f;
		a = vp.Height;
		viewport._22 = -a / 2.0f;
		viewport._33 = vp.MaxZ - vp.MinZ;
		viewport._41 = vp.X + vp.Width / 2.0f;
		viewport._42 = vp.Y + vp.Height / 2.0f;
		viewport._43 = vp.MinZ;

		D3DXVec3TransformCoord(&result, &result, &view);
		D3DXVec3TransformCoord(&result, &result, &projection);
		D3DXVec3TransformCoord(&result, &result, &viewport);

		return result;
	}
}