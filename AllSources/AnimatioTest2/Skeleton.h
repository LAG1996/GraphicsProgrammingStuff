#pragma once
#ifndef SKELETON_H
#define SKELETON_H
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
	std::vector<Bone*> List_Bones;
	glm::mat4 globalInverseTransform;
	std::vector<glm::mat4> BoneMatrices;

	float time;
	float start_time;
	float end_time;

	Animation * active_animation;
	Animation * idle_animation;

	bool anim_play;
	bool anim_loop;

	Skeleton()
	{
		this->globalInverseTransform = glm::mat4();

		this->time = this->start_time = this->end_time;
		this->active_animation = nullptr;
		this->idle_animation = nullptr;

		anim_play = false;
		anim_loop = false;
	}

	Skeleton(std::vector<Bone*> in_bones, glm::mat4 in_globalInverseTransform)
	{
		Init(in_bones, in_globalInverseTransform);
	}

	void Init(std::vector<Bone*> in_bones, glm::mat4 in_globalInverseTransform)
	{
		List_Bones = in_bones;
		this->globalInverseTransform = in_globalInverseTransform;

		for (int i = 0; i < List_Bones.size(); i++)
		{
			List_Bones.at(i)->parent_skeleton = this;
		}
	}

	Bone* FindBone(std::string name)
	{
		for (int i = 0; i < List_Bones.size(); i++)
		{
			if (List_Bones.at(i)->name == name)
			{
				return List_Bones.at(i);
			}
		}
		return nullptr;
	}

	//Gather all of the bone's global transformations and place them in order in a 4x4 matrix
	void UpdateBoneMatsVector()
	{
		if (List_Bones.size() == 0)
			return;

		//Clear the bone transformation vector so we don't have any weird artifacts.
		BoneMatrices.clear();

		int i = 0;
		for (int i = 0; i < 100; i++)
		{
			//Pass the identity matrix if we are past the number of bones in the actual skeleton
			if (i > List_Bones.size() - 1)
			{
				BoneMatrices.push_back(glm::mat4(1.0f));
			}
			else
			{
				std::cout << "[\n " << List_Bones.at(i)->offset_matrix[0][0] << ", " << List_Bones.at(i)->offset_matrix[0][1] << ", " << List_Bones.at(i)->offset_matrix[0][2] << ", " << List_Bones.at(i)->offset_matrix[0][3] << std::endl
					<< List_Bones.at(i)->offset_matrix[1][0] << ", " << List_Bones.at(i)->offset_matrix[1][2] << ", " << List_Bones.at(i)->offset_matrix[1][3] << std::endl
					<< List_Bones.at(i)->offset_matrix[2][0] << ", " << List_Bones.at(i)->offset_matrix[2][1] << ", " << List_Bones.at(i)->offset_matrix[2][2] << ", " << List_Bones.at(i)->offset_matrix[2][3] << std::endl
					<< List_Bones.at(i)->offset_matrix[3][0] << ", " << List_Bones.at(i)->offset_matrix[3][1] << ", " << List_Bones.at(i)->offset_matrix[3][2] << ", " << List_Bones.at(i)->offset_matrix[3][3] << std::endl
					<< "]" << std::endl;
				//If we are not past the number of bones in the skeleton,
				//Multiply the concatenated transofmrs of the bone's parents with the bone's transformation.
				//A note: the bone's matrix is NOT in world space. This is in local space. Conversion with world space
				//will happen in the shader stage.
				glm::mat4 concatenated_transformation = List_Bones.at(i)->GetParentTransforms() * AiToGLMMat4(List_Bones.at(i)->node->mTransformation);
				//Transform the bone back to the origin by multiplying it with its offset matrix, then multiply it by the concatenated transforms,
				//and then by the global inverse transform. Note that the multiplication happens backwards.
				BoneMatrices.push_back(globalInverseTransform * concatenated_transformation * List_Bones.at(i)->offset_matrix);
			}
		}
	}

	void PlayAnimation(Animation * anim, bool loop, bool reset_to_start)
	{
		//If there's an animation currently playing
		if (active_animation != nullptr)
		{
			//If the new animation is more important than the current animation, run the new animation
			if (anim->priority < active_animation->priority)
			{
				active_animation = anim;
			}
			else
				return;
		}
		else
			active_animation = anim;

		this->start_time = active_animation->start_time;
		this->end_time = active_animation->end_time;
		this->anim_play = true;
		this->anim_loop = loop;

		//If an animation should restart upon playing, reset it
		if (reset_to_start)
			this->time = active_animation->start_time;
	}

	void StopAnimation()
	{
		this->time = this->end_time;
		this->active_animation = nullptr;
		this->anim_play = false;
	}

	//Set the idle animation
	void SetIdleAnimation(Animation* idle)
	{
		idle_animation = idle;
	}

	void Update(float deltaTime)
	{
		UpdateBoneMatsVector();

		if (!anim_play)
			return;

		//Update the time variable by adding a delta time
		this->time += deltaTime;

		//Make sure that the time is not less than the current animation's start time
		if (this->time < this->start_time)
			this->time = this->start_time;

		//The time cannot be greater than the animation's end time. If it is, and looping is not set, then loop the animation
		if (this->time > this->end_time)
		{
			if (this->anim_loop)
				this->time = this->start_time;
			else
				StopAnimation();
		}

		for (int i = 0; i < List_Bones.size(); i++)
			List_Bones.at(i)->UpdateKeyframeTransform(this->time);
	}
};
#endif