#pragma once

#include<sstream>

#include<string>
#include<vector>

#include<GLEW/GL/glew.h>
// GLM Mathematics
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

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
	GLuint ID;
	GLuint BaseVertex;

	Mesh(vector<Vertex> ListVertices, vector<Texture> ListTextures, vector<GLuint> indices)
	{
		this->ListVertices = ListVertices;
		this->ListTextures = ListTextures;
		this->indices = indices;

		this->_SetUpMesh();
	}

	void Draw(Shader shader, glm::vec3 color, float shininess);
};


