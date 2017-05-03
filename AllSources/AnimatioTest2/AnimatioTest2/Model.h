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

	void Draw(Shader shader, glm::vec3 Color, float shininess, float deltaTime)
	{
		if (is_rigged)
		{
			Update(deltaTime);
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

	void Update(float deltaTime)
	{
		skeleton->Update(deltaTime);
	}

	void AddAnimation(Animation*, bool);
	Animation * FindAnimation(std::string);
	void PlayAnimation(Animation*, bool, bool);
	void StopAnimation();

private:
	unsigned int _num_vertices;

	//List of bones in this model
	std::vector<Bone*> List_Bones;
	//This model's skeleton
	Skeleton * skeleton;
	//This model's animations
	std::vector<Animation*> List_Animations;

	//Our aiScene
	const aiScene * scene;

	//Global Inverse Transform
	glm::mat4 globalInverseTransform;

	std::vector<aiNode*> List_aiNodes;
	std::vector<aiNodeAnim*> List_NodeAnim;

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

	//Find a bone in the list of bones in this model
	Bone * FindBone(std::string name)
	{	
		for (int i = 0; i < List_Bones.size(); i++)
		{
			if (List_Bones.at(i)->name == name)
				return List_Bones.at(i);
		}

		return nullptr;
	}

	//Find an aiNode in the list of aiNodes in this model
	aiNode* FindAiNode(std::string name)
	{
		for (int i = 0; i < List_aiNodes.size(); i++)
		{
			if (List_aiNodes.at(i)->mName.data == name)
				return List_aiNodes.at(i);
		}

		std::cout << "Node " << name << "not found" << std::endl;

		return nullptr;
	}

	//Find the animation with the name we pass in. 
	aiNodeAnim* FindAiNodeAnim(std::string name)
	{
		for (int i = 0; i < List_NodeAnim.size(); i++)
		{
			if (List_NodeAnim.at(i)->mNodeName.data == name)
				return List_NodeAnim.at(i);
		}

		return nullptr;
	}

	//Find the id of a bone withinthe bones vector
	int FindBoneIDByName(std::string name)
	{
		for (int i = 0; i < List_Bones.size(); i++)
		{
			if (List_Bones.at(i)->name == name)
				return i;
		}

		return -1;
	}
};
#endif