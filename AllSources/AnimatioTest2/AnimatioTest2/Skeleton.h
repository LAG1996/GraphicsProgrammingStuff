#pragma once
#include "MatrixConversions.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<assimp\Importer.hpp>
#include<assimp\scene.h>
#include<assimp\postprocess.h>

#include<map>
#include<vector>
#include<string>

#include "Bone.h"

class Skeleton
{
public:
	std::map<std::string, Bone*> bones;
	glm::mat4 globalInverseTransform;
	std::vector<glm::mat4> boneTrans;

	Skeleton()
	{}

	Skeleton(std::map<std::string, Bone*> in_bones, glm::mat4 in_globalInverseTransform)
	{
		Init(in_bones, in_globalInverseTransform);
	}

	void Init(std::map<std::string, Bone*> in_bones, glm::mat4 in_globalInverseTransform)
	{
		std::cout << "FUCK" << std::endl;
		
		bones.insert(in_bones.begin(), in_bones.end());

		/*
		for (std::map<std::string, Bone*>::iterator it = in_bones.begin(); it != in_bones.end(); it++)
		{
			bones[it->first] = it->second;
		}

		std::cout << "FUCK FUCK FUCK" << std::endl;

		for (std::map<std::string, Bone*>::iterator it2 = bones.begin(); it2 != bones.end(); it2++)
		{
			it2->second->parent_skeleton = this;
		}*/

		std::cout << bones.size() << std::endl;
	}

	Bone* FindBone(std::string name)
	{
		if (bones.find(name) == bones.end())
		{
			return nullptr;
		}
		
		return bones[name];
	}

	//Gather all of the bone's global transformations and place them in order in a 4x4 matrix
	void UpdateBoneMatsVector()
	{
		if (bones.size() == 0)
			return;

		//Clear the bone transformation vector so we don't have any weird artifacts.
		boneTrans.clear();

		int i = 0;
		for (std::map<std::string, Bone*>::iterator it = bones.begin(); it != bones.end(); it++)
		{
			//Pass the identity matrix if we are past the number of bones in the actual skeleton
			if (i > bones.size() - 1)
			{
				boneTrans.push_back(glm::mat4(1.0f));
			}
			else
			{
				//If we are not past the number of bones in the skeleton,
				//Multiply the concatenated transofmrs of the bone's parents with the bone's transformation.
				//A note: the bone's matrix is NOT in world space. This is in local space. Conversion with world space
				//will happen in the shader stage.
				glm::mat4 concatenated_transformation = it->second->GetParentTransforms() * AiToGLMMat4(it->second->node->mTransformation);
				//Transform the bone back to the origin by multiplying it with its offset matrix, then multiply it by the concatenated transforms,
				//and then by the global inverse transform. Note that the multiplication happens backwards.
				boneTrans.push_back(globalInverseTransform * concatenated_transformation * it->second->offset_matrix);
			}
			i++;
		}
	}

	void Update()
	{
		UpdateBoneMatsVector();
	}
};