#version 330

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

in vec2 texCoord;

out vec4 color;

uniform Material material;

void main()
{
	color = vec4(texture(material.diffuse, texCoord));
}