#pragma once

#include <cstdint>
#include <cassert>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "Vec.h"
#include "Utils.h"

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

	/// Returns whether this polygon and the given polygon overlap. Polygons are
	/// still considered to be overlapping if they only overlap on their boundary.
	bool overlaps(const ConvexPolygon &b, HDC hdc) const;

private:
	int mNumVertices;
	Vec2* mVertices;
};