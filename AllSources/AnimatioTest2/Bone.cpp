#include "Bone.h"
#include<iostream>
//Return the index of the last keyframe in the array of keyframes in the bone's animNode at the inserted time
unsigned int Bone::FindPositionKeyframe(float time)
{
	for (unsigned int i = 0; i < this->animNode->mNumPositionKeys - 1; i++)
	{
		if (time < (float)this->animNode->mPositionKeys[i + 1].mTime)
			return i;
	}

	return 0;
}

//Return the index of the last keyframe in the array of keyframes in the bone's animNode at the inserted time
unsigned int Bone::FindRotationKeyframe(float time)
{
	for (unsigned int i = 0; i < this->animNode->mNumRotationKeys - 1; i++)
	{
		if (time < (float)animNode->mRotationKeys[i + 1].mTime)
			return i;
	}

	return 0;
}

//Calculated the position of the bone at the inputted time from the last keyframe
glm::vec3 Bone::CalcInterpolatedPosition(float time)
{
	//If there is only one position keyframe, then just return the position at that keyframe.
	if (this->animNode->mNumPositionKeys == 1)
	{
		aiVector3D assimp_val = this->animNode->mPositionKeys[0].mValue;
		glm::vec3 val(assimp_val.x, assimp_val.y, assimp_val.z);
		return val;
	}

	//There are multiple position keyframes. Find the last keyframe and the next keyframe and interpolate between them
	unsigned int LastPositionIndex = FindPositionKeyframe(time);
	unsigned int NextPositionIndex = LastPositionIndex + 1;
	
	float DeltaTime = this->animNode->mPositionKeys[NextPositionIndex].mTime - this->animNode->mPositionKeys[LastPositionIndex].mTime; //Time between frames
	float Factor = (time - (float)animNode->mPositionKeys[LastPositionIndex].mTime / DeltaTime); //Percentage of the way between frames

	//Get the start and end positions of the bone
	const aiVector3D StartPosition = this->animNode->mPositionKeys[LastPositionIndex].mValue;
	const aiVector3D EndPosition = this->animNode->mPositionKeys[NextPositionIndex].mValue;

	this->startPosition = glm::vec3(StartPosition.x, StartPosition.y, StartPosition.z);
	this->endPosition = glm::vec3(EndPosition.x, EndPosition.y, EndPosition.z);

	glm::vec3 interpolated_value = glm::mix(this->startPosition, this->endPosition, Factor);

	return interpolated_value;
}

glm::quat Bone::CalcInterpolatedRotation(float time)
{
	//If there is only one rotation keyframe, then just return the position at that keyframe.
	if (this->animNode->mNumRotationKeys == 1)
	{
		aiQuaternion assimp_val = this->animNode->mRotationKeys[0].mValue;
		glm::quat val(assimp_val.w, assimp_val.x, assimp_val.y, assimp_val.z);
		return val;
	}

	//There are multiple rotation keyframes. Find the last keyframe and the next keyframe and interpolate between them
	unsigned int LastRotationIndex = FindRotationKeyframe(time);
	unsigned int NextRotationIndex = LastRotationIndex + 1;

	float DeltaTime = this->animNode->mPositionKeys[NextRotationIndex].mTime - this->animNode->mPositionKeys[LastRotationIndex].mTime; //Time between frames
	float Factor = (time - (float)animNode->mPositionKeys[LastRotationIndex].mTime / DeltaTime); //Percentage of the way between frames

	//Get the start and end positions of the bone
	const aiQuaternion& StartRotation = this->animNode->mRotationKeys[LastRotationIndex].mValue;
	const aiQuaternion& EndRotation = this->animNode->mRotationKeys[NextRotationIndex].mValue;

	this->startRotation = glm::quat(StartRotation.w, StartRotation.x, StartRotation.y, StartRotation.z);
	this->endRotation = glm::quat(EndRotation.w, EndRotation.x, EndRotation.y, EndRotation.z);

	glm::quat interpolated_value = glm::slerp(this->startRotation, this->endRotation, Factor);

	return interpolated_value;
}

void Bone::UpdateKeyframeTransform(float time)
{
	if (this->animNode == nullptr)
		return;

	this->currentPosition = CalcInterpolatedPosition(time);
	this->currentRotation = CalcInterpolatedRotation(time);
	scale = glm::vec3(1.0); //Add in support for scaling later on...

	glm::mat4 mat;
    glm::translate(mat, this->currentPosition);
	mat *= glm::mat4_cast(this->currentRotation);
	glm::scale(mat, scale);

	node->mTransformation = GLMMat4ToAi(mat);
}