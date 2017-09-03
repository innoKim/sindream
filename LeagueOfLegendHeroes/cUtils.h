#pragma once
#include <cmath>

namespace MY_UTIL
{
	// 거리 구한다
	float GetDistance(float startX, float startY, float endX, float endY)
	{
		float x = endX - startX;
		float y = endY - endY;
		return sqrt(x * x + y * y);
	}

	// 두 지점의 각을 구한다
	float GetAngle(float startX, float startY, float endX, float endY);
}