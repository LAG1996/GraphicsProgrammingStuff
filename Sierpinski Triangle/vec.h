#pragma once
class vec2
{
public:
	double x;
	double y;

	vec2() { x = 0; y = 0; }
	vec2(double x, double y)
	{
		this->x = x;
		this->y = y;
	}

	vec2 operator+(vec2 b)
	{
		return vec2(this->x + b.x, this->y + b.y);
	}

	vec2 operator/(double scalar)
	{
		return vec2(this->x / scalar, this->y / scalar);
	}

	vec2 operator*(double scalar)
	{
		return vec2(this->x * scalar, this->y * scalar);
	}
};