#pragma once
#include "Mesh.h"
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
		_amount_bones = 0;
		_amount_vertices = 0;
	}

	void Draw(Shader shader, glm::vec3 color, float shininess)
	{
		for (int i = 0; i < this->_meshes.size(); i++)
		{
			this->_meshes[i].Draw(shader, color, shininess);
		}
	}

private:
	struct VertexBoneData
	{
		vector<GLuint> IDs;
		vector<GLfloat> Weights;
	};

	struct BoneInfo
	{
		GLuint ID;
		aiMatrix4x4 BoneOffset;
		aiMatrix4x4 FinalTransformation;
	};


	// Model Data
	vector<Mesh> _meshes; //List of meshes in the model
	string _directory; //The model's directory
	vector<Texture> _textures_loaded;
	vector<VertexBoneData> _Bones;


	std::map<aiMesh*, VertexBoneData> MapMeshToVertexBoneData;
	std::map<GLuint, BoneInfo> MapIDToBoneInfo;
	std::map<string, GLuint> MapNameToBoneID;
	std::map<string, aiAnimation*> MapNameToAnimation;
	
	GLuint _amount_vertices;
	GLuint _amount_bones;
	const aiScene * scene;
	aiMatrix4x4 globalInverseTransform;
	Assimp::Importer importer;
	// Private Functions
	void _LoadModel(string path); //Retrieve model found in path
	void _ProcessNode(aiNode* node, const aiScene* scene); //Process all nodes in the scene graph imported in _LoadModel
	void _ProcessMesh(aiMesh* mesh, const aiScene* scene, GLuint MeshID);
	void _ProcessBones(aiMesh* mesh, GLuint MeshID);

	void BoneTransform(float TimeInSeconds, vector<aiMatrix4x4>& Transforms, string AnimName);
	void ReadNodeHeirarchy(float AnimationTime, const aiNode* node, aiMatrix4x4 ParentTransform, string AnimName);

	vector<Texture> _LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	GLuint _TextureFromFile(const char*, string);

};