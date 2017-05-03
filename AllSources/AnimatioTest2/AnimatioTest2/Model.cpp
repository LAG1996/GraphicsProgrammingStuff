#include "Model.h"
#include "SOIL/SOIL.h"

#include <string>

//Retrieve the model from a file
void Model::_LoadModel(std::string path)
{
	//Initialize the importer
	Assimp::Importer importer;
	importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);

	this->scene = importer.GetOrphanedScene();

	//Error check
	if (!this->scene || this->scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !this->scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	else
		std::cout << "SUCCESS::ASSIMP::FILE READ SUCCESSFUL" << std::endl;

	this->globalInverseTransform = glm::inverse(AiToGLMMat4(this->scene->mRootNode->mTransformation));

	//Process all nodes recursively
	this->_directory = path.substr(0, path.find_last_of('/')); //Get this model's directory
	this->_ProcessNode(this->scene->mRootNode, this->scene);
	this->_AnimNodeProcess();
	this->_ProcessBones();
	
}

//Recursively process all nodes in the imported model starting at a node and then all of its children
void Model::_ProcessNode(aiNode* node, const aiScene* scene)
{
	List_aiNodes.push_back(node);
	//Process all the node's meshes
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->_meshes.push_back(this->_ProcessMesh(mesh, scene));
	}

	//Recurse to child nodes
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		this->_ProcessNode(node->mChildren[i], scene);
	}
}

//Process the data gathered from assimp into a mesh
//We do this by getting all the important parts of a mesh (vertices, indices, and textures), and then generating a mesh out of it
//(i.e. creating a new Mesh object)
Mesh Model::_ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	std::vector<unsigned int> boneIDs;
	std::vector<float> boneWeights;

	const int WEIGHTS_PER_VERTEX = 4;
	
	int boneArraySize = mesh->mNumVertices*WEIGHTS_PER_VERTEX;

	boneIDs.resize(boneArraySize);
	boneWeights.resize(boneArraySize);

	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		//Process the vertex's position, normals and texture coordinates from assimp's data
		//Get the coordinates of each vertex
		glm::vec3 vect;
		vect.x = mesh->mVertices[i].x;
		vect.y = mesh->mVertices[i].y;
		vect.z = mesh->mVertices[i].z;
		vertex.Position = vect;

		//Get the normals of each vertex
		vect.x = mesh->mNormals[i].x;
		vect.y = mesh->mNormals[i].y;
		vect.z = mesh->mNormals[i].z;
		vertex.Normal = vect;

		//Get the texture coordinates for each vertex if the mesh contains any
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}
	//Process all indices
	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	//Process material if there is any
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial * material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = this->_LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture> specularMaps = this->_LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	//Process bones
	for (int i = 0; i < mesh->mNumBones; i++)
	{
		aiBone* aiBone = mesh->mBones[i];

		for (int j = 0; j < aiBone->mNumWeights; j++)
		{
			aiVertexWeight weight = aiBone->mWeights[j];
			unsigned int vertexStart = weight.mVertexId*WEIGHTS_PER_VERTEX;

			for (int k = 0; k < WEIGHTS_PER_VERTEX; k++)
			{
				if (boneWeights.at(vertexStart + k) == 0)
				{
					boneWeights.at(vertexStart + k) = weight.mWeight;
					boneIDs.at(vertexStart + k) = i;

					vertices.at(weight.mVertexId).id[k] = i;

					vertices.at(weight.mVertexId).Weights[k] = weight.mWeight;

					break;
				}
			}	
		}
	}

	return Mesh(vertices, textures, indices, this);
}

void Model::_ProcessBones()
{
	for (int i = 0; i < this->scene->mNumMeshes; i++)
	{
		for (int j = 0; j < this->scene->mMeshes[i]->mNumBones; j++)
		{
			//Store information about bones into the Bone class
			std::string name = this->scene->mMeshes[i]->mBones[j]->mName.data;
			glm::mat4 b_transform = glm::transpose(AiToGLMMat4(this->scene->mMeshes[i]->mBones[j]->mOffsetMatrix));

			Bone * bone = new Bone(&_meshes.at(i), i, name, b_transform);
			bone->node = FindAiNode(name);
			bone->animNode = FindAiNodeAnim(name);

			if (bone->animNode == nullptr)
			{
				std::cout << "No animation for " << bone->name << std::endl;
			}
			else
			{
				std::cout << "Animation node: " << bone->animNode->mNodeName.data << " found for bone: " << bone->name << std::endl;
			}

			List_Bones.push_back(bone);
		}
	}

	for (int i = 0; i < List_Bones.size(); i++)
	{
		std::string b_name = List_Bones.at(i)->name;
		std::string p_name = FindAiNode(b_name)->mParent->mName.data;


		Bone* p_bone = FindBone(p_name);
		List_Bones.at(i)->parent = p_bone;

		if (p_bone != nullptr)
		{
			std::cout << "Current Bone: " << List_Bones.at(i)->name << " | Parent Bone: " << p_bone->name << std::endl;
		}
	}

	if (_meshes.size() > 0)
	{
		_meshes.at(0).Skeleton_Affecting_Me = new Skeleton(List_Bones, this->globalInverseTransform);
	}
}

std::vector<Texture> Model::_LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		GLboolean skip = false;
		//Check if this texture was already loaded
		for (GLuint j = 0; j < _textures_loaded.size(); j++)
		{
			if (std::strcmp(_textures_loaded[j].path.C_Str(), str.C_Str()) == 0)
			{
				//If this texture was already loaded, just push it to the texture vector
				textures.push_back(_textures_loaded[j]);
				skip = true;
				break;
			}
		}

		//If this texture was not loaded, load it
		if (!skip)
		{
			Texture tex;
			tex.id = this->_TextureFromFile(str.C_Str(), this->_directory);
			tex.type = typeName;
			tex.path = str;
			textures.push_back(tex);
			this->_textures_loaded.push_back(tex);
		}
		
	}
	return textures;
}

GLuint Model::_TextureFromFile(const char* path, std::string directory)
{
	//Generate texture ID and load texture data
	std::string filename = std::string(path);

	filename = directory + '/' + filename;

	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height;
	unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	//Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	//Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return textureID;
}

void Model::_AnimNodeProcess()
{
	if (this->scene->mNumAnimations == 0)
		return;

	for (int i = 0; i < this->scene->mAnimations[0]->mNumChannels; i++)
	{
		List_NodeAnim.push_back(this->scene->mAnimations[0]->mChannels[i]);
	}
}

void Model::AddAnimation(Animation * in_anim, bool is_idle)
{
	List_Animations.push_back(in_anim);

	if (is_idle)
	{
		skeleton->SetIdleAnimation(in_anim);
	}
}

Animation * Model::FindAnimation(std::string anim_to_find)
{
	for (int i = 0; i < List_Animations.size(); i++)
	{
		if (List_Animations.at(i)->name == anim_to_find)
			return List_Animations.at(i);
	}

	return nullptr;
}

void Model::PlayAnimation(Animation * anim, bool loop, bool reset_to_start)
{
	skeleton->PlayAnimation(anim, loop, reset_to_start);
}

void Model::StopAnimation()
{
	skeleton->StopAnimation();
}