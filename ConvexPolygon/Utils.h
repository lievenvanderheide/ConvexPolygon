#pragma once

#include "Vec.h"

inline bool isCounterClockwise(Vec2 a, Vec2 b, Vec2 c)
{
	Vec2 leg1 = b - a;
	Vec2 leg2 = c - a;
	return leg1.perpDot(leg2) > 0;
}

inline bool isClockwise(Vec2 a, Vec2 b, Vec2 c)
{
	Vec2 leg1 = b - a;
	Vec2 leg2 = c - a;
	return leg1.perpDot(leg2) < 0;
}

inline int predModulo(int i, int m)
{
	assert(i >= 0 && i < m);
	return i == 0 ? m - 1 : i - 1;
}

inline int succModulo(int i, int m)
{
	assert(i >= 0 && i < m);
	int ret = i + 1;
	return ret != m ? ret : 0;
}