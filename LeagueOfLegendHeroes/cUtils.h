#pragma once
#include <cmath>

namespace MY_UTIL
{
	// �Ÿ� ���Ѵ�
	float GetDistance(float startX, float startY, float endX, float endY)
	{
		float x = endX - startX;
		float y = endY - endY;
		return sqrt(x * x + y * y);
	}

	// �� ������ ���� ���Ѵ�
	float GetAngle(float startX, float startY, float endX, float endY);
}