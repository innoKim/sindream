#pragma once
#include <cmath>

namespace MY_UTIL
{
	// 거리 구한다
	float GetDistance(float startX, float startY, float endX, float endY);

	// 두 지점의 각을 구한다
	float GetAngle(float startX, float startY, float endX, float endY);
}