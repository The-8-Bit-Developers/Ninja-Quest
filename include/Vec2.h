#pragma once

/* 
	Vector class with operator overloads
*/
class Vec2
{
public:
	float x;
	float y;

	Vec2() : x(0.0f), y(0.0f) {}
	Vec2(float _x, float _y) : x(_x), y(_y) {}

	Vec2 operator + (const Vec2& rhs) { return Vec2(this->x + rhs.x, this->y + rhs.y); }
	Vec2 operator - (const Vec2& rhs) { return Vec2(this->x - rhs.x, this->y - rhs.y); }
	Vec2 operator * (const Vec2& rhs) { return Vec2(this->x * rhs.x, this->y * rhs.y); }
	Vec2 operator / (const Vec2& rhs) { return Vec2(this->x / rhs.x, this->y / rhs.y); }
	Vec2 operator * (const float& rhs) { return Vec2(this->x * rhs, this->y * rhs); }
	Vec2 operator / (const float& rhs) { return Vec2(this->x / rhs, this->y / rhs); }

	Vec2& operator += (const Vec2& rhs) { this->x += rhs.x; this->y += rhs.y; return *this; }
	Vec2& operator -= (const Vec2& rhs) { this->x -= rhs.x; this->y -= rhs.y; return *this; }

	Vec2 normalise()
	{
		float m = sqrt(x*x + y * y);
		x /= m;
		y /= m;
		return *this;
	}

	float dot(Vec2 rhs)
	{
		return x * rhs.x + y * rhs.y;
	}

	explicit operator std::string() const
	{
		return std::to_string(x) + std::string(", ") + std::to_string(y);
	}
};