#include"Mesh.h"
#include"MatrixConversions.h"
#include<string>

class Bone
{
public:
	string name;
	int id;
	Mesh* mesh;

	aiNode* node;

	//The bone's corresponding aiNodeAnim, where we'll get info for this bone's current animation.
	aiNodeAnim* animNode;

	Bone * parent; //Pointer to a parent bone
	glm::mat4 parent_transforms; //A concatenation of all transforms before this bone
	glm::mat4 offset_matrix; //The offset matrix for the bone

	Skeleton* parent_skeleton;

	Bone(){ name = ""; id = -2; }
	Bone(Mesh* in_mesh, int in_id, string in_name, aiMatrix4x4 in_off_mat)
	{
		mesh = in_mesh;
		id = in_id;
		name = in_name;
		offset_matrix = AiToGLMMat4(in_off_mat);
	}

	Bone(Mesh* in_mesh, int in_id, string in_name, glm::mat4 in_off_mat)
	{
		mesh = in_mesh;
		id = in_id;
		name = in_name;
		offset_matrix = in_off_mat;
	}
	
	//Concatenate parent transforms. Vital for calculating bone's final transformation
	glm::mat4 GetParentTransforms()
	{
		Bone* b = parent; //Begin with parent. We will then recurse upward
		vector<glm::mat4> transforms;

		while (b != nullptr)
		{
			glm::mat4 temp_trans = AiToGLMMat4(b->node->mTransformation);
			transforms.push_back(temp_trans);

			b = b->parent;
		}

		glm::mat4 concatenated_transforms;

		//Multiply in reverse, as is the rule of thumb with matrices
		for (int i = transforms.size() - 1; i >= 0; i--)
		{
			concatenated_transforms *= transforms.at(i);
		}

		return concatenated_transforms; //Return the total transformation of all this bone's parents
	}

};