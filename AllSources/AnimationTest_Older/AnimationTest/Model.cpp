#include "Model.h"
#include "SOIL/SOIL.h"

//Retrieve the model from a file
void Model::_LoadModel(string path)
{
	//Initialize the importer
	this->scene = this->importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	//Error check
	if (!this->scene || this->scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !this->scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << this->importer.GetErrorString() << std::endl;
		return;
	}
	else
		std::cout << "SUCCESS::ASSIMP::FILE READ SUCCESSFUL" << std::endl;

	this->globalInverseTransform = this->scene->mRootNode->mTransformation;
	this->globalInverseTransform.Inverse();

	//Process all nodes recursively
	this->_directory = path.substr(0, path.find_last_of('/')); //Get this model's directory

	for (GLuint i = 0; i < this->scene->mNumAnimations; i++)
	{
		aiAnimation * animation = this->scene->mAnimations[i];
		this->MapNameToAnimation[animation->mName.data] = animation;
	}


	this->_ProcessNode(this->scene->mRootNode, this->scene);
}

//Recursively process all nodes in the imported model starting at a node and then all of its children
void Model::_ProcessNode(aiNode* node, const aiScene* scene)
{

	//Process all the node's meshes
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->_ProcessMesh(mesh, scene, this->_meshes.size() + 1);
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
void Model::_ProcessMesh(aiMesh* mesh, const aiScene* scene, GLuint MeshID)
{
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

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
		vector<Texture> diffuseMaps = this->_LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		vector<Texture> specularMaps = this->_LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}
	Mesh m = Mesh(vertices, textures, indices);
	m.ID = this->_meshes.size();
	m.BaseVertex = _amount_vertices;

	_amount_vertices += mesh->mNumVertices;

	this->_meshes.push_back(m);

	//Process bones if there are any
	if (mesh->HasBones())
	{
		_ProcessBones(mesh, m.ID);
	}
}

void Model::_ProcessBones(aiMesh* mesh, GLuint MeshID)
{
	for (int i = 0; i < mesh->mNumBones; i++)
	{
		string BoneName(mesh->mBones[i]->mName.data);

		GLuint bone_index = 0;

		if (MapNameToBoneID.find(BoneName) != MapNameToBoneID.end())
		{
			bone_index = MapNameToBoneID[BoneName];
		}
		else
		{
			bone_index = _amount_bones++;
			BoneInfo NewBone;
			
			MapIDToBoneInfo[bone_index] = NewBone;
		}
			
		MapNameToBoneID[BoneName] = bone_index;
		MapIDToBoneInfo[bone_index].BoneOffset = mesh->mBones[i]->mOffsetMatrix;

		VertexBoneData NewBone;
		for (GLuint j = 0; j < mesh->mBones[i]->mNumWeights; j++)
		{
			GLuint VertID = _meshes[MeshID].BaseVertex + mesh->mBones[i]->mWeights[j].mVertexId;
			float Weight = mesh->mBones[i]->mWeights[j].mWeight;

			
			NewBone.IDs.push_back(VertID);
			NewBone.Weights.push_back(Weight);
		}

		_Bones.push_back(NewBone);
	}
}

void Model::BoneTransform(float TimeInSeconds, vector<aiMatrix4x4>& Transforms, string AnimName)
{
	aiMatrix4x4 Identity = aiMatrix4x4();
	
	float TicksPerSecond = this->scene->mAnimations[0]->mTicksPerSecond;

	if (TicksPerSecond == 0)
	{
		TicksPerSecond = 25.0f;
	}

	float TimeInTicks = TimeInSeconds * TimeInTicks;
	float AnimationTime = fmod(TimeInTicks, this->scene->mAnimations[0]->mDuration);

	ReadNodeHeirarchy(AnimationTime, this->scene->mRootNode, Identity, AnimName);

	for (GLuint i = 0; i < _amount_bones; i++)
	{
		Transforms.push_back(this->MapIDToBoneInfo[i].FinalTransformation);
	}
}

void Model::ReadNodeHeirarchy(float AnimationTime, const aiNode* node, aiMatrix4x4 ParentTransform, string AnimName)
{
	string NodeName(node->mName.data);

	const aiAnimation* animation = MapNameToAnimation[AnimName];

	aiMatrix4x4 NodeTransformation = node->mTransformation;

	if (animation)
	{
		//Interpolate scaling and generate scaling transformation matrix
		aiVector3D Scaling;
		
	}
}

void Model::CalcInterpolatedScaling(aiQuaternion & Out, float AnimationTime, const aiNodeAnim* nodeAnim)
{

}



vector<Texture> Model::_LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
	vector<Texture> textures;
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

GLuint Model::_TextureFromFile(const char* path, string directory)
{
	//Generate texture ID and load texture data
	string filename = string(path);

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