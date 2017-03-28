#ifndef VEC4_H
#define VEC4_H

#include<cmath>
#include<GL/glut.h>

class Vec4
{
public:
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat w;

	Vec4()
	{
		x = 0;
		y = 0;
		z = 0;
		w = 0;
	}

	Vec4(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	GLfloat Magnitude()
	{
		return sqrt(x*x + y*y + z*z);
	}

	Vec4 Normalized()
	{
		if (Magnitude() > 0)
			return Vec4(x / Magnitude(), y / Magnitude(), z / Magnitude(), w);
		else
			return *this;
	}

	static GLfloat Dot(Vec4 A, Vec4 B)
	{
		return A.x*B.x + A.y*B.y + A.z*B.z;
	}

	GLfloat * ToList()
	{
		GLfloat stuff[] = { x, y, z, w };

		return stuff;
	}

	Vec4 operator+(Vec4 A)
	{
		return Vec4(x + A.x, y + A.y, z + A.z, w);
	}

	Vec4 operator*(Vec4 A)
	{
		Vec4 Cross;
		Cross.x = y*A.z - z * A.y;
		Cross.y = z*A.x - x*A.z;
		Cross.z = x*A.y - y*A.x;
		Cross.w = 1.0f;

		return Cross;
	}

	Vec4 operator*(GLfloat s)
	{
		return Vec4(x*s, y*s, z*s, w);
	}

	Vec4 operator=(GLfloat * list)
	{
		x = list[0];
		y = list[1];
		z = list[2];
		w = list[3];

		return *this;
	}

	Vec4 operator=(Vec4 A)
	{
		x = A.x;
		y = A.y;
		z = A.z;
		w = A.w;

		return *this;
	}

	Vec4 operator+=(Vec4 A)
	{
		x = x + A.x;
		y = y + A.y;
		z = z + A.z;

		return *this;
	}

	Vec4 operator+= (GLfloat * list)
	{
		x = list[0] + x;
		y = list[1] + y;
		z = list[2] + z;

		return *this;
	}

	Vec4 operator-=(Vec4 A)
	{
		x = x - A.x;
		y = y - A.y;
		z = z - A.z;

		return *this;
	}

	Vec4 operator-(Vec4 A)
	{
		return Vec4(x - A.x, y - A.y, z - A.z, w);
	}

	static Vec4 Zero()
	{
		return Vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	static Vec4 Up()
	{
		return Vec4(0.0f, 1.0f, 0.0f, 1.0f);
	}
};

#endif