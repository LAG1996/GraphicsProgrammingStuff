#pragma once
#include "Mesh.h"
#include<assimp\Importer.hpp>
#include<assimp\scene.h>
#include<assimp\postprocess.h>

class Model
{
public:

	Model(GLchar* path)
	{
		this->_LoadModel(path);
	}

	void Draw(Shader shader)
	{
		for (int i = 0; i < this->_meshes.size(); i++)
		{
			this->_meshes[i].Draw(shader);
		}
	}

private:
	// Model Data
	vector<Mesh> _meshes;
	string _directory;
	// Functions
	void _LoadModel(string path);
	void _ProcessNode(aiNode* node, const aiScene* scene);
	Mesh _ProcessMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> _LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};