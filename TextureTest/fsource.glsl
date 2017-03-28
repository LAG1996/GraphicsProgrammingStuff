#version 330 core

in vec2 myTex;
out vec4 fColor;

uniform sampler2D ourTexture;

void main()
{
	fColor = texture(ourTexture, myTex);
}