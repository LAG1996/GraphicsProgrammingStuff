#version 330
layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoord;
layout (location = 3) in ivec4 IDs;
layout (location = 4) in vec4 Weights; 

const int MAX_BONES = 100;

out vec3 position;
out vec3 normal;
out vec2 texCoord;
out vec4 weights;
out ivec4 ids;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 mBones[MAX_BONES];

void main()
{
	mat4 Bone_Matrix = mBones[IDs[0]] * Weights[0];
	Bone_Matrix += mBones[IDs[1]] * Weights[1];
	Bone_Matrix += mBones[IDs[2]] * Weights[2];
	Bone_Matrix += mBones[IDs[3]] * Weights[3];

	gl_Position = projection * view  * model * Bone_Matrix * vec4(Position, 1.0f);
	position = vec3(model * vec4(Position, 1.0));
	texCoord = TexCoord;
	normal = mat3(transpose(inverse(model))) * Normal;

	weights = Weights;
	ids = IDs;
}