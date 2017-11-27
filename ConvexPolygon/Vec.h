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

	Vec2 operator + (Vec2 b) const
	{
		return Vec2(mX + b.mX, mY + b.mY);
	}

	Vec2 operator - (Vec2 b) const
	{
		return Vec2(mX - b.mX, mY - b.mY);
	}

	int64_t dot(Vec2 b) const
	{
		return (int64_t)mX * (int64_t)b.mX + 
			(int64_t)mY * (int64_t)b.mY;
	}

	int64_t perpDot(Vec2 b) const
	{
		return (int64_t)mX * (int64_t)b.mY - 
			(int64_t)mY * (int64_t)b.mX;
	}

	void setNormal(Vec2 a, Vec2 b)
	{
		mX = b.mY - a.mY;
		mY = a.mX - b.mX;
	}

	static bool lexLT(Vec2 a, Vec2 b)
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