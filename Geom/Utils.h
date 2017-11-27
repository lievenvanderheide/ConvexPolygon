#pragma once

#include "Vec.h"

/// Returns whether the triangle formed by the vertices a, b and c has counter
/// clockwise winding.
inline bool isCounterClockwise(Vec2 a, Vec2 b, Vec2 c)
{
	Vec2 leg1 = b - a;
	Vec2 leg2 = c - a;
	return leg1.perpDot(leg2) > 0;
}

/// Returns whether the triangle formed by the vertices a, b and c has clockwise winding.
inline bool isClockwise(Vec2 a, Vec2 b, Vec2 c)
{
	Vec2 leg1 = b - a;
	Vec2 leg2 = c - a;
	return leg1.perpDot(leg2) < 0;
}

/// Returns the predecessor of i, modulo m. i must already lie in the 0 <= i < m range.
inline int predModulo(int i, int m)
{
	assert(i >= 0 && i < m);
	return i == 0 ? m - 1 : i - 1;
}

/// Returns the successor of i, modulo m. i must already lie in the 0 <= i < m range.
inline int succModulo(int i, int m)
{
	assert(i >= 0 && i < m);
	int ret = i + 1;
	return ret != m ? ret : 0;
}