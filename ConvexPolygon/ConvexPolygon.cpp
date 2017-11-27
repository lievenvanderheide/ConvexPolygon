#include <algorithm>
#include <vector>

#include "ConvexPolygon.h"

ConvexPolygon::ConvexPolygon()
	: mNumVertices(0), mVertices(nullptr)
{
}

ConvexPolygon::~ConvexPolygon()
{
	delete [] mVertices;
}

void ConvexPolygon::setConvexHull(Vec2* points, int numPoints)
{
	std::sort(points, points + numPoints, Vec2::lexLT);

	Vec2 leftMost = points[0];
	Vec2 rightMost = points[numPoints - 1];

	Vec2* tmp = new Vec2[numPoints];
	Vec2* lowerArcBegin = tmp;
	Vec2* lowerArcEnd = tmp;
	Vec2* upperArcBegin = tmp + numPoints;
	Vec2* upperArcEnd = tmp + numPoints;

	*(lowerArcEnd++) = points[0];
	*(--upperArcBegin) = points[0];
	for(int i = 1; i < numPoints - 1; i++)
	{
		if(isCounterClockwise(leftMost, rightMost, points[i]))
		{
			while(upperArcEnd - upperArcBegin > 1)
			{
				if(isClockwise(*(upperArcBegin + 1), *upperArcBegin, points[i]))
				{
					break;
				}

				upperArcBegin++;
			}

			*(--upperArcBegin) = points[i];
		}
		else
		{
			while(lowerArcEnd - lowerArcBegin > 1)
			{
				if(isCounterClockwise(*(lowerArcEnd - 2), *(lowerArcEnd - 1), points[i]))
				{
					break;
				}

				lowerArcEnd--;
			}

			*(lowerArcEnd++) = points[i];
		}
	}

	while(upperArcEnd - upperArcBegin > 1)
	{
		if(isClockwise(*(upperArcBegin + 1), *upperArcBegin, rightMost))
		{
			break;
		}

		upperArcBegin++;
	}

	while(lowerArcEnd - lowerArcBegin > 1)
	{
		if(isCounterClockwise(*(lowerArcEnd - 2), *(lowerArcEnd - 1), rightMost))
		{
			break;
		}

		lowerArcEnd--;
	}

	// Delete the previous vertices just in case the ConvexPolygon wasn't empty
	// when setConvexHull was called.
	delete [] mVertices;

	int lowerArcSize = lowerArcEnd - lowerArcBegin;
	int upperArcSize = upperArcEnd - upperArcBegin;
	mNumVertices = lowerArcSize + upperArcSize;
	mVertices = new Vec2[mNumVertices];
	
	std::copy(lowerArcBegin, lowerArcEnd, mVertices);
	mVertices[lowerArcSize] = rightMost;
	std::copy(upperArcBegin, upperArcEnd - 1, mVertices + lowerArcSize + 1);

	delete [] tmp;
}

bool ConvexPolygon::pointInPolygon(Vec2 point) const
{
	Vec2 prev = mVertices[mNumVertices - 1];
	for(int i = 0; i < mNumVertices; i++)
	{
		if(isClockwise(point, prev, mVertices[i]))
		{
			return false;
		}

		prev = mVertices[i];
	}

	return true;
}

bool ConvexPolygon::overlaps(const ConvexPolygon &b) const
{
	// A rotating calipers based algorithm which tests whether the two convex
	// polygons overlap.
	//
	// From the separating axis theorem, we know that the two polygons do not
	// overlap if and only if there's an axis such that the projects of both
	// polygons onto this axis don't overlap. We also know that if such an axis
	// exists can be found, then the normal of at least one of the edges of one
	// of the polygons would be such an axis.
	//
	// Now the naive approach would be to try each edge normal as axis, then
	// project both polygons onto this axis and see if these projections
	// overlap, but we can do much better:
	//
	// Given any edge 'aEdge' of polygon A, we know that:
	// - All vertices of polygon A lie behind this edge, that is, the signed
	//   distance from each vertex to the line through aEdge is negative.
	// - If we know the vertex of polygon B with the lowest signed distance to
	//   aEdge, then we only need to test whether this vertex has positive or
	//   negative signed distance to know whether there's an overlap.
	// - The vertex with the lowest signed distance is the one which lies
	//   between two adjacent edge of polygon B, where the incoming edge moves
	//   towards aEdge and the outgoing edge away from it, or, alternatively,
	//   the orientations from -bIncomingEdge.mNormal to aEdge.mNormal and from
	//   aEdge.mNormal to bOutgoingEdge.mNormal are both positive (note that the
	//   normals of B are negated).
	//
	// Now this of course suggests a rotating calipers algorithm, which is
	// implemented below. What we will do is that we iterate over the vertices
	// of both polygons in parallel, while maintaining the following invariant:
	//
	// - If aCur and bCur are the current vertices of polygons A and B
	//   respectively, then the edges which end in aCur and bCur have an
	//   orientation which comes before the orientations of both the edges which
	//   start in aCur and bCur. Note that we negate the normal of edges from
	//   polygon B before determining orientation.
	
	int aNumVerts = mNumVertices;
	int bNumVerts = b.mNumVertices;

	// The first step is to find starting vertices. It's clear that the if we
	// pick the leftmost vertex of polygon A as the first vertex of polygon A
	// and the rightmost vertex of polygon B as the first vertex of polygon B,
	// then the invariant described above holds.
	int aFirst = 0;
	for(int i = 1; i < aNumVerts; i++)
	{
		if(Vec2::lexLT(mVertices[i], mVertices[aFirst]))
		{
			aFirst = i;
		}
	}

	int bFirst = 0;
	for(int i = 1; i < bNumVerts; i++)
	{
		if(Vec2::lexLT(b.mVertices[bFirst], b.mVertices[i]))
		{
			bFirst = i;
		}
	}

	int aCur = aFirst;
	int bCur = bFirst;
	int aNext = succModulo(aCur, aNumVerts);
	int bNext = succModulo(bCur, bNumVerts);

	NormalAndOffset aEdge, bEdge;
	aEdge.setLine(mVertices[aCur], mVertices[aNext]);
	bEdge.setLine(b.mVertices[bCur], b.mVertices[bNext]);

	// The main loop. Note that each iteration handles one of the edges, so if
	// we do a total of aNumVerts + bNumVerts iterations, we know that we must
	// have handled all edges.
	int numIterations = aNumVerts + bNumVerts;
	for(int i = 0; i < numIterations; i++)
	{
		// We have to decide whether to advance aCur or bCur depending on
		// whether the orientation of the outgoing edge from aCur comes before
		// or after the orientation of the outgoing edge from bCur.
		if(aEdge.mNormal.perpDot(bEdge.mNormal) < 0)
		{
			// We'll advancing aCur, but first we will do the separating axis
			// test for the edge originating from aCur (that is, the edge
			// between the current vertex and the one we're advancing to). Since
			// we know that
			// - The incoming edge to bCur comes before aEdge (from the invariant).
			// - The outgoing edge from bCur comes after aEdge (we just tested that).
			// we know that the vertex we have to test against aEdge is bCur.
			if(aEdge.pointInFront(b.mVertices[bCur]))
			{
				// We found a separating axis, so return 'no overlap'.
				return false;
			}

			// Advance aCur and compute a new aEdge. It's clear that this keeps
			// the invariant.
			aCur = aNext;
			aNext = succModulo(aNext, aNumVerts);
			aEdge.setLine(mVertices[aCur], mVertices[aNext]);
		}
		else
		{
			// We'll advancing bCur, but first we will do the separating axis
			// test for the edge originating from bCur (that is, the edge
			// between the current vertex and the one we're advancing to). Since
			// we know that
			// - The incoming edge to aCur comes before bEdge (from the invariant).
			// - The outgoing edge from aCur comes after bEdge (we just tested that).
			// we know that the vertex we have to test against bEdge is aCur.
			if(bEdge.pointInFront(mVertices[aCur]))
			{
				return false;
			}

			// Advance bCur and compute a new bEdge. It's clear that this keeps
			// the invariant.
			bCur = bNext;
			bNext = succModulo(bNext, bNumVerts);
			bEdge.setLine(mVertices[bCur], mVertices[bNext]);
		}
	}

	return true;
}

