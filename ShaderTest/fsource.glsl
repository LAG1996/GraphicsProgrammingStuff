#version 330 core

in vec3 myColor;
out vec4 fColor;

void main()
{
	fColor = vec4(myColor, 1.0);
}