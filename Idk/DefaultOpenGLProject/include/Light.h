#include"Vec4.h"

typedef Vec4 Color4;

struct PointLight
{
	Vec4 position;
	Color4 color_diffuse;
	Color4 color_specular;
	Color4 color_ambient;
};

struct AmbientLight
{
	Color4 color_diffuse;
	Color4 color_specular;
	Color4 color_ambient;
};