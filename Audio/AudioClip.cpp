#include "AudioClip.h"

void AudioClip::Initialization(std::string fileName, FMOD::Sound* sound, FMOD::Channel* channel, bool looping, bool is3D, float volume, glm::vec3 position, glm::vec3 velocity)
{
	this->fileName = fileName;
	this->soundClip = sound;
	this->channel = channel;
	this->isLooping = looping;
	this->is3DSound = is3D;
	this->volume = volume;
	this->position = { position.x, position.y, position.z };
	this->velocity = { velocity.x, velocity.y, velocity.z };
}

void AudioClip::SetIsLooping(bool loop)
{
	if (soundClip != nullptr)
	{
		isLooping = loop;
		soundClip->setMode(GenerateMode());
	}
}

void AudioClip::SetIs3D(bool is3D)
{
	if (soundClip != nullptr)
	{
		is3DSound = is3D;
		soundClip->setMode(GenerateMode());
	}
}

void AudioClip::SetSound(FMOD::Sound* sound, FMOD::Channel* channel)
{
	if (sound != nullptr && channel != nullptr)
	{
		if (soundClip != nullptr)
		{
			soundClip->release();
		}
		soundClip = sound;
		this->channel = channel;
	}
}

void AudioClip::SetChannel(FMOD::Channel* channel)
{
	if (channel != nullptr)
	{
		this->channel = channel;
	}
}

void AudioClip::SetAttributes()
{
	if (channel != nullptr)
	{
		channel->set3DAttributes(&position, &velocity);
	}
}

// Aux method to fill the modes variable
FMOD_MODE AudioClip::GenerateMode() const
{
	FMOD_MODE mode;
	mode = is3DSound ? FMOD_3D : FMOD_DEFAULT;
	if (isLooping)
	{
		mode |= FMOD_LOOP_NORMAL;
	}
	return mode;
}