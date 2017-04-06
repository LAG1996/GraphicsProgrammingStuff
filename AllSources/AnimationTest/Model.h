#pragma once
#include "Mesh.h"
#include "Bone.h"
#include<assimp\Importer.hpp>
#include<assimp\scene.h>
#include<assimp\postprocess.h>

#include <map>

class Model
{
public:

	Model(GLchar* path)
	{
		this->_LoadModel(path);
		this->_AnimNodeProcess();
	}

	void Draw(Shader shader, glm::vec3 Color, float shininess)
	{
		for (int i = 0; i < this->_meshes.size(); i++)
		{
			this->_meshes[i].Draw(shader, Color, shininess);
		}
	}

private:
	unsigned int _num_vertices;

	//List of bones in this model
	std::map<string, Bone *> NameToBone;

	//Our aiScene
	const aiScene * scene;

	//Global Inverse Transform
	glm::mat4 globalInverseTransform;

	std::map<string, aiNode*> NameToNode;
	std::map<string, aiNodeAnim*> NameToNodeAnim;

	// Model Data
	vector<Mesh> _meshes; //List of meshes in the model
	string _directory; //The model's directory
	vector<Texture> _textures_loaded;
	// Private Functions
	void _LoadModel(string path); //Retrieve model found in path
	void _ProcessNode(aiNode* node, const aiScene* scene); //Process all nodes in the scene graph imported in _LoadModel
	Mesh _ProcessMesh(aiMesh* mesh, const aiScene* scene);
	void _ProcessBones();
	vector<Texture> _LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	GLuint _TextureFromFile(const char*, string);
	void _AnimNodeProcess();

	Bone * FindBone(string name)
	{
		if (NameToBone.find(name) == NameToBone.end())
			return nullptr;
		else
			return NameToBone[name];
	}

	aiNode* FindAiNode(string name)
	{
		if (NameToNode.find(name) == NameToNode.end())
			return nullptr;
		else
			return NameToNode[name];
	}

	aiNodeAnim* FindAiNodeAnim(string name)
	{
		if (NameToNodeAnim.find(name) == NameToNodeAnim.end())
			return nullptr;
		else
			return NameToNodeAnim[name];
	}

	int FindBoneIDByName(string name)
	{
		if (NameToBone.find(name) == NameToBone.end())
			return -1;
		else
			return NameToBone[name]->id;
	}
};