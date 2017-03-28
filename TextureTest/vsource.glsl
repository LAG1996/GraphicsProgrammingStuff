#version 330 core

layout (location = 0) in vec2 vPosition;
layout (location = 2) in vec2 texCoord;

out vec2 myTex;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectMat;

void main()
{
	gl_Position =  projectMat * viewMat * modelMat * vec4(vPosition, 0.0, 1.0);
	myTex = vec2(texCoord.x, 1.0 - texCoord.y);
}