#pragma once

#include <fmod.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <fmod_errors.h>
#include "../Logger/Logger.h"
#include "AudioClip.h"

class AudioManager
{
public:
	void Initialization();
	void LoadAudio(std::string fileName);
	void PlayAudio(std::string fileName);
	void PauseOrResumeAudio(std::string fileName);
	void SetVolume(std::string fileName, float volume);
	void RemoveAudioClip(std::string fileName);
	void SetAudioListener(glm::vec3 pos, glm::vec3 vel, glm::vec3 frwd, glm::vec3 up);
	void Update();
	void Cleanup();
private:
	FMOD::System* fmodSystem;
	const int maxChannels = 512;
	std::vector<AudioClip> audioClips;
};

