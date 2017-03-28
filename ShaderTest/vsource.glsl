#version 330 core

in vec2 vPosition;
in vec3 vColor;

out vec3 myColor;

void main()
{
	gl_Position = vec4(vPosition, 0.0, 1.0);
	myColor = vColor;
}