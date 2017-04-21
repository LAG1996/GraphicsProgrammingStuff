#pragma once
#ifndef MODEL_H
#define MODEL_H

#include<assimp\Importer.hpp>
#include<assimp\scene.h>
#include<assimp\postprocess.h>

#include<string>
#include <map>
#include <vector>

#include "Mesh.h"
#include "Bone.h"
#include "Skeleton.h"

class Shader;

class Model
{
public:
	//Public variables
	bool is_rigged;

	//Public Functions and Constructors
	Model(GLchar* path)
	{
		this->_LoadModel(path); //Big one. Load the model from a file in the path
		is_rigged = false; //Assume the model is not rigged. We will determine if it is somewhere else

		if (this->_meshes.size() != 0)
		{
			skeleton = _meshes.at(0).Skeleton_Affecting_Me;
		}
	}

	void Draw(Shader shader, glm::vec3 Color, float shininess)
	{
		if (is_rigged)
		{
			UpdateSkeleton();
		}

		for (int i = 0; i < this->_meshes.size(); i++)
		{
			this->_meshes[i].Draw(shader, Color, shininess);
		}
	}

	Skeleton * GetSkeleton()
	{
		return this->skeleton;
	}

	void UpdateSkeleton()
	{
		skeleton->Update();
	}

private:
	unsigned int _num_vertices;

	//List of bones in this model
	std::map<std::string, Bone *> NameToBone;
	//This model's skeleton
	Skeleton * skeleton;

	//Our aiScene
	const aiScene * scene;

	//Global Inverse Transform
	glm::mat4 globalInverseTransform;

	std::map<std::string, aiNode*> NameToNode;
	std::map<std::string, aiNodeAnim*> NameToNodeAnim;

	// Model Data
	std::vector<Mesh> _meshes; //List of meshes in the model
	std::string _directory; //The model's directory
	std::vector<Texture> _textures_loaded;
	// Private Functions
	void _LoadModel(std::string path); //Retrieve model found in path
	void _ProcessNode(aiNode* node, const aiScene* scene); //Process all nodes in the scene graph imported in _LoadModel
	Mesh _ProcessMesh(aiMesh* mesh, const aiScene* scene);
	void _ProcessBones();
	std::vector<Texture> _LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	GLuint _TextureFromFile(const char*, std::string);
	void _AnimNodeProcess();

	Bone * FindBone(std::string name)
	{
		if (NameToBone.find(name) == NameToBone.end())
			return nullptr;
		else
			return NameToBone[name];
	}

	aiNode* FindAiNode(std::string name)
	{
		if (NameToNode.find(name) == NameToNode.end())
			return nullptr;
		else
			return NameToNode[name];
	}

	aiNodeAnim* FindAiNodeAnim(std::string name)
	{
		if (NameToNodeAnim.find(name) == NameToNodeAnim.end())
			return nullptr;
		else
			return NameToNodeAnim[name];
	}

	int FindBoneIDByName(std::string name)
	{
		if (NameToBone.find(name) == NameToBone.end())
			return -1;
		else
			return NameToBone[name]->id;
	}
};
#endif