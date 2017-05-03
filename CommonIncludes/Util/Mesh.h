#pragma once
#ifndef MESH_H
#define MESH_H
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

class Model;
class Skeleton;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;

	float Weights[4];
	unsigned int id[4];
};

struct Texture {
	GLuint id;
	std::string type;
	aiString path;
};

class Mesh
{
private:
	std::vector<Vertex> ListVertices;
	std::vector<Texture> ListTextures;
	std::vector<GLuint> indices;

	GLuint VAO, VBO, EBO;

	void _SetUpMesh();

public:

	unsigned int BaseVertex;
	Skeleton * Skeleton_Affecting_Me;
	Model * Model_I_Belong_To;

	Mesh(std::vector<Vertex> ListVertices, std::vector<Texture> ListTextures, std::vector<GLuint> indices, Model * Parent)
	{
		this->ListVertices = ListVertices;
		this->ListTextures = ListTextures;
		this->indices = indices;

		this->_SetUpMesh();

		this->Model_I_Belong_To = Parent;
	}

	void Draw(Shader shader, glm::vec3 Color, float shininess, bool instanced, int count);
};
#endif