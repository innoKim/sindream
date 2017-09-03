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

}