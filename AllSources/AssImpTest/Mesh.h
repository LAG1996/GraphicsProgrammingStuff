#pragma once

#include<sstream>

#include<string>
#include<vector>

#include<GL/glew.h>
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

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
	Mesh(vector<Vertex> ListVertices, vector<Texture> ListTextures, vector<GLuint> indices)
	{
		this->ListVertices = ListVertices;
		this->ListTextures = ListTextures;
		this->indices = indices;

		this->_SetUpMesh();
	}

	void Draw(Shader shader);
};


