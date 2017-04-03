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
	vector<Mesh> _meshes; //List of meshes in the model
	string _directory; //The model's directory
	vector<Texture> _textures_loaded;
	// Private Functions
	void _LoadModel(string path); //Retrieve model found in path
	void _ProcessNode(aiNode* node, const aiScene* scene); //Process all nodes in the scene graph imported in _LoadModel
	Mesh _ProcessMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> _LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	GLuint _TextureFromFile(const char*, string);
};