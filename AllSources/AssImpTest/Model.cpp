#include "Model.h"
#include "SOIL.h"

//Retrieve the model from a file
void Model::_LoadModel(string path)
{
	//Initialize the importer
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	//Error check
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	//Process all nodes recursively
	this->_directory = path.substr(0, path.find_last_of('/')); //Get this model's directory

	this->_ProcessNode(scene->mRootNode, scene);
}

//Recursively process all nodes in the imported model starting at a node and then all of its children
void Model::_ProcessNode(aiNode* node, const aiScene* scene)
{
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

	return Mesh(vertices, textures, indices);
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
		}
		
	}
	return textures;
}

GLuint Model::_TextureFromFile(const char* path, string directory)
{
	//Generate texture ID and load texture data
	string filename = string(path);
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