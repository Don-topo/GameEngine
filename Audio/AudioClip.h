#pragma once

#include <string>
#include "../Logger/Logger.h"
#include "glm/glm.hpp"
#include "fmod.hpp"

class AudioClip
{
public:
	void Initialization(std::string fileName, FMOD::Sound* sound, FMOD::Channel* channel, bool looping = false, bool is3D = false, float volume = 0.5f, glm::vec3 position = { 0.0f, 0.0f, 0.0f }, glm::vec3 velocity = { 0.0f, 0.0f, 0.f });
	// Setters
	void SetIsLooping(bool loop);
	void SetIs3D(bool is3D);
	void SetSound(FMOD::Sound* sound, FMOD::Channel* channel);
	void SetChannel(FMOD::Channel* channel);
	void SetPosition(glm::vec3 newPosition) { position = { newPosition.x, newPosition.y, newPosition.z }; };
	void SetVelocity(glm::vec3 newVelocity) { velocity = { newVelocity.x, newVelocity.y, newVelocity.z }; };
	void SetAttributes();

	// Getters
	FMOD::Sound* GetSound() { return soundClip; };
	FMOD::Channel* GetChannel() { return channel; };
	std::string GetFileName() { return fileName; };

	bool AudioIsLooping() const { return isLooping; };
	bool AudioIs3D() const { return is3DSound; };

private:
	std::string fileName;
	bool isLooping;
	bool is3DSound;
	FMOD::Sound* soundClip;
	FMOD::Channel* channel;
	float volume;
	FMOD_VECTOR position;
	FMOD_VECTOR velocity;
	FMOD_MODE GenerateMode() const;
};