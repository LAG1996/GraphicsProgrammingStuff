#pragma once

#include<sstream>

#include<string>
#include<vector>

#include<GLEW/GL/glew.h>
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<assimp\Importer.hpp>
#include<assimp\scene.h>
#include<assimp\postprocess.h>

#include "Util/Shader.h"

#define string std::string
#define stringstream std::stringstream
#define vector std::vector

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;

	float Weights[4];
	unsigned int id[4];
};

struct Texture {
	GLuint id;
	string type;
	aiString path;
};

class Mesh
{
private:
	vector<Vertex> ListVertices;
	vector<Texture> ListTextures;
	vector<GLuint> indices;

	GLuint VAO, VBO, EBO;

	void _SetUpMesh();

public:

	unsigned int BaseVertex;

	Mesh(vector<Vertex> ListVertices, vector<Texture> ListTextures, vector<GLuint> indices)
	{
		this->ListVertices = ListVertices;
		this->ListTextures = ListTextures;
		this->indices = indices;

		this->_SetUpMesh();
	}

	void Draw(Shader shader, glm::vec3 Color, float shininess);
};


