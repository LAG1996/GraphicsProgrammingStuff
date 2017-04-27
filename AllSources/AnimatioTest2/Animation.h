#pragma once
#ifndef ANIM_H
#define ANIM_H

#include <GLM/glm.hpp>
#include <string>

static float TimeToFrame(float time, float frameRate)
{
	return time / (1.0f/frameRate);
}

static glm::vec2 FramesToTime(glm::vec2 start_and_end_frame, float frameRate)
{
	return start_and_end_frame * (1.0f/frameRate);
}

class Animation
{
public:
	std::string name;
	float start_time;
	float end_time;
	int priority;

	Animation()
	{
		start_time = end_time = priority = 0;
	}

	Animation(std::string name, glm::vec2 times, int priority)
	{
		this->name = name;
		this->start_time = times.x;
		this->end_time = times.y;
		this->priority = priority;
	}
};
#endif