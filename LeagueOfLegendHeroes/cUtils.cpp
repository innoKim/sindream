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

	//테스트 결과 성공
	bool PtInTri(ST_TRIANGLE& triangle, D3DXVECTOR2& point)
	{
		D3DXVECTOR3 ab = triangle.b - triangle.a;
		D3DXVECTOR3 ac = triangle.c - triangle.a;
		D3DXVECTOR3 ad = point - triangle.a;

		//ab, ac 벡터로 이루어진 삼각형 이 점d 를 포함하기위해서는
		// k*ab+l*ac = ad 의 k,l에대한 연립방정식의 해가 k>=0, l>= 0, k+l<=1을 만족할떄 성립한다.
		
		//연립방정식의 일반해는 다음과 같으므로
		//float k = (ac.y*ad.x-ac.x*ad.y) / (ab.x*ac.y - ab.y*ac.x);
		//float l = (ab.x*ad.y- ab.y*ad.x) / (ab.x*ac.y - ab.y*ac.x);
		//먼저 (ab.x*ac.y - ab.y*ac.x); 가 0이 아니어야한다. 
		//이는 두 벡터가 평행함을 의미하므로 삼각형이면 성립하지 않는다 따라서 고려할 필요는 없지만 혹시나해서

		float denominator = (ab.x*ac.y - ab.y*ac.x);
		if (denominator <= FLT_EPSILON) return false;

		float k = (ac.y*ad.x - ac.x*ad.y) / (ab.x*ac.y - ab.y*ac.x);
		float l = (ab.x*ad.y - ab.y*ad.x) / (ab.x*ac.y - ab.y*ac.x);

		if (k < 0) return false;
		if (l < 0)	return false;
		if (k + l > 1) return false;
		
		//위의 세조건을 피하면 점 d는 삼각형 abc 안에 위치한다.
		return true;
	}

	bool PtInSquare(ST_SQUARE& square, D3DXVECTOR2& point)
	{
		if (square.a.x > point.x) return false;
		if (square.d.x < point.x) return false;
		if (square.a.y > point.y) return false;
		if (square.d.y < point.y) return false;
		return true;
	}

	bool TriVsSq(ST_TRIANGLE& tri,ST_SQUARE& sq)
	{
		//사각형 내부에 삼각형의 점 하나가 들어가있거나.
		if(PtInSquare(sq, tri.a))return true;
		if(PtInSquare(sq, tri.b))return true;
		if(PtInSquare(sq, tri.c))return true;
		
		//삼각형 내부에 사각형의 점 하나가 들어가있거나.
		if(PtInTri(tri, sq.a))return true;
		if(PtInTri(tri, sq.b))return true;
		if(PtInTri(tri, sq.c))return true;
		if(PtInTri(tri, sq.d))return true;

		return false;
	}
}