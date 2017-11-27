#pragma once

// A 2 dimensional vector, using integers coordinates.
class Vec2
{
public:
	Vec2() { }
	Vec2(int32_t x, int32_t y)
		: mX(x), mY(y)
	{
	}

	/// Returns the sum of this vector and the vector b.
	Vec2 operator + (Vec2 b) const
	{
		return Vec2(mX + b.mX, mY + b.mY);
	}

	/// Returns the vector difference of this vector and the vector b.
	Vec2 operator - (Vec2 b) const
	{
		return Vec2(mX - b.mX, mY - b.mY);
	}

	/// Returns the dot product of this vector with the given vector.
	int64_t dot(Vec2 b) const
	{
		return (int64_t)mX * (int64_t)b.mX + 
			(int64_t)mY * (int64_t)b.mY;
	}

	/// Returns the perp dot product of this vector. The perp dot product equals
	/// the dot product of this vector, rotated by 90 degrees in the counter
	/// clockwise direction, and the vector in b.
	int64_t perpDot(Vec2 b) const
	{
		return (int64_t)mX * (int64_t)b.mY - 
			(int64_t)mY * (int64_t)b.mX;
	}

	/// Sets this vector to the normal of the line through the given two
	/// vectors, that is, it takes the vector from a to b and rotates it by 90
	/// degrees in the counter clockwise direction.
	void setNormal(Vec2 a, Vec2 b)
	{
		mX = b.mY - a.mY;
		mY = a.mX - b.mX;
	}

	/// A lexicographical less-than comparison. This function first compares the
	/// x coordinates of both vectors, and uses the y coordinates to break ties.
	static bool lexLessThan(Vec2 a, Vec2 b)
	{
		if(a.mX != b.mX)
		{
			return a.mX < b.mX;
		}
		else
		{
			return a.mY < b.mY;
		}
	}

	int32_t mX;
	int32_t mY;
};