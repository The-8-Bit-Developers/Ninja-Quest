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
};