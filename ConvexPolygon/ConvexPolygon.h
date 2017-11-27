#pragma once

#include <cstdint>
#include <cassert>

#include "Vec.h"

class NormalAndOffset
{
public:
	void setLine(Vec2 a, Vec2 b)
	{
		mNormal.setNormal(a, b);
		mOffset = mNormal.dot(a);
	}

	bool pointInFront(Vec2 pt) const
	{
		return pt.dot(mNormal) > mOffset;
	}

	Vec2 mNormal;
	int64_t mOffset;
};

class Rect
{
public:
	Rect() { }
	Rect(Vec2 min, Vec2 max)
		: mMin(min), mMax(max)
	{
	}

	Vec2 mMin;
	Vec2 mMax;
};

enum class Orientation : uint8_t
{
	CounterClockwise,
	Clockwise,
	Degenerate,
};

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

/// A convex polygon, represented as a list of vertices in counter clockwise order.
class ConvexPolygon
{
public:
	ConvexPolygon();
	~ConvexPolygon();

	/// Returns the number of vertices in this polygon.
	int getNumVertices() const { return mNumVertices; }

	/// Returns the vertex at index i.
	Vec2 getVertex(int i) const
	{
		assert(i >= 0 && i < mNumVertices);
		return mVertices[i];
	}

	/// Computes the convex hull of the given set of points. Note that the
	/// function will sort the points in \p points lexicographically, so callers
	/// shouldn't rely on the contents of the points array staying the same.
	void setConvexHull(Vec2* points, int numPoints);

	/// Returns whether the given point lies inside this con
	bool pointInPolygon(Vec2 point) const;

	/// Returns whether this polygon and the given polygon overlap. Polygons are still considered to be
	/// overlapping if they only overlap on their boundary.
	bool overlaps(const ConvexPolygon &b) const;

private:
	int mNumVertices;
	Vec2* mVertices;
};