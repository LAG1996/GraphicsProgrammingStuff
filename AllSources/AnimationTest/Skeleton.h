#include "Mesh.h"
#include "Bone.h"
#include "MatrixConversions.h"

#include<map>

class Skeleton
{
public:
	std::map<string, Bone*> bones;
	glm::mat4 globalInverseTransform;
	vector<glm::mat4> boneTrans;

	Skeleton()
	{}

	Skeleton(std::map<string, Bone*> in_bones, glm::mat4 in_globalInverseTransform)
	{
		Init(in_bones, in_globalInverseTransform);
	}

	void Init(std::map<string, Bone*> in_bones, glm::mat4 in_globalInverseTransform)
	{
		bones = in_bones;

		for (std::map<string, Bone*>::iterator it = bones.begin(); it != bones.end(); it++)
		{
			it->second->parent_skeleton = this;
		}
	}

	Bone* FindBone(string name)
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

		boneTrans.clear();

		int i = 0;
		for (std::map<string, Bone*>::iterator it = bones.begin(); it != bones.end(); it++)
		{
			if (i > bones.size() - 1)
			{
				boneTrans.push_back(glm::mat4(1.0f));
			}
			else
			{
				glm::mat4 concatenated_transformation = it->second->GetParentTransforms() * AiToGLMMat4(it->second->node->mTransformation);
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