#ifndef PRISM_H
#define PRISM_H

#include"Vec4.h"

class R_Prism
{
private:
	Vec4 Center;
	GLfloat height;
	GLfloat width;
	GLfloat depth;
	Vec4 * points;
	Vec4 * normals;

	void SetPoints()
	{
		points[0] = Vec4(Center.x - width / 2, Center.y - height / 2, Center.z - depth / 2, 0.0f);
		points[1] = Vec4(Center.x + width / 2, Center.y - height / 2, Center.z - depth / 2, 0.0f);
		points[2] = Vec4(Center.x + width / 2, Center.y - height / 2, Center.z + depth / 2, 0.0f);
		points[3] = Vec4(Center.x - width / 2, Center.y - height / 2, Center.z + depth / 2, 0.0f);
		points[4] = Vec4(Center.x - width / 2, Center.y + height / 2, Center.z - depth / 2, 0.0f);
		points[5] = Vec4(Center.x + width / 2, Center.y + height / 2, Center.z - depth / 2, 0.0f);
		points[6] = Vec4(Center.x + width / 2, Center.y + height / 2, Center.z + depth / 2, 0.0f);
		points[7] = Vec4(Center.x - width / 2, Center.y + height / 2, Center.z + depth / 2, 0.0f);
	}

public:
	R_Prism() {
		Center = Vec4();
		height = 0;
		width = 0;
		depth = 0;
	}
	R_Prism(Vec4 Pos, GLfloat h, GLfloat w, GLfloat d)
	{
		Center = Pos;
		height = h;
		width = w;
		depth = d;

		points = new Vec4[8];

		SetPoints();

		normals = new Vec4[8];

		normals[0] = { -1.0f, -1.0f, -1.0f, 0.0f };
		normals[1] = { 1.0f, -1.0f, -1.0f, 0.0f };
		normals[2] = { 1.0f, -1.0f, 1.0f, 0.0f };
		normals[3] = { -1.0f, -1.0f, 1.0f, 0.0f };
		normals[4] = { -1.0f, 1.0f, -1.0f, 0.0f };
		normals[5] = { 1.0f, 1.0f, -1.0f, 0.0f };
		normals[6] = { 1.0f, 1.0f, 1.0f, 0.0f };
		normals[7] = { -1.0f, 1.0f, 1.0f, 0.0f };

	}

	Vec4 GetCenter()
	{
		return Center;
	}

	void SetCenter(Vec4 A)
	{
		Center = A;
	}

	void Translate(Vec4 A)
	{
		this->Center = this->Center + A;
	}

	Vec4 * GetPoints()
	{
		SetPoints();
		return points;
	}

	Vec4 * GetNormals()
	{
		return normals;
	}
};

#endif