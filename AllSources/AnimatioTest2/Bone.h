#pragma once
#ifndef BONE_H
#define BONE_H
#include<string>
#include<vector>

#include"MatrixConversions.h"
#include "Animation.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Mesh;
class Skeleton;

class Bone
{
public:
	std::string name;
	int id;
	Mesh* mesh;

	aiNode* node;

	//The bone's corresponding aiNodeAnim, where we'll get info for this bone's current animation.
	aiNodeAnim* animNode;

	Bone * parent; //Pointer to a parent bone
	glm::mat4 parent_transforms; //A concatenation of all transforms before this bone
	glm::mat4 offset_matrix; //The offset matrix for the bone

	Skeleton* parent_skeleton; //The skeleton this bone belongs to

	//Some data about this bone in a single keyframe
	glm::vec3 currentPosition;
	glm::quat currentRotation;
	glm::vec3 scale;
	glm::vec3 startPosition;
	glm::vec3 endPosition;
	glm::quat startRotation;
	glm::quat endRotation;

	//Some functions to define in Bone.cpp
	unsigned int FindPositionKeyframe(float);
	glm::vec3 CalcInterpolatedPosition(float);
	unsigned int FindRotationKeyframe(float);
	glm::quat CalcInterpolatedRotation(float);
	void UpdateKeyframeTransform(float);

	//Begin functions defined in this file below

	Bone(){ name = ""; id = -2; }
	Bone(Mesh* in_mesh, int in_id, std::string in_name, aiMatrix4x4 in_off_mat)
	{
		mesh = in_mesh;
		id = in_id;
		name = in_name;
		offset_matrix = AiToGLMMat4(in_off_mat);
	}

	Bone(Mesh* in_mesh, int in_id, std::string in_name, glm::mat4 in_off_mat)
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
		std::vector<glm::mat4> transforms;

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
#endif