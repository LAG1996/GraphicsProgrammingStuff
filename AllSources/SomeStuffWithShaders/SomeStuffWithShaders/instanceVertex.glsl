#version 330
layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoord;
layout (location = 3) in ivec4 IDs;
layout (location = 4) in vec4 Weights; 
layout(location = 5) in mat4 instanceMatrix;

out vec3 position;
out vec3 normal;
out vec2 texCoord;
out vec4 weights;
out ivec4 ids;

uniform mat4 projection;
uniform mat4 view;

void main()
{

	gl_Position = projection * view * instanceMatrix * vec4(Position, 1.0f);
	position = vec3(instanceMatrix * vec4(Position, 1.0));
	texCoord = TexCoord;
	normal = mat3(transpose(inverse(instanceMatrix))) * Normal;

	weights = Weights;
	ids = IDs;
}