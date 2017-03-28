#include "Vec4.h"
#include "GL/glut.h"

struct Material
{	
	Vec4 ambient;
	Vec4 diffuse;
	Vec4 specular;
	Vec4 emission;
	const GLfloat * shininess;
};