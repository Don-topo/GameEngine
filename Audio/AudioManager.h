#pragma once

#include <fmod.hpp>
#include <string>
#include <vector>
#include "../Logger/Logger.h"
#include "AudioClip.h"

class AudioManager
{
public:
	void Initialization();
	void LoadAudio(std::string fileName);
	void PlayAudio(std::string fileName);
	void PauseOrResumeAudio(std::string filename);
	void RemoveAudioClip(std::string fileName);
	void SetAudioListener();
	void Update();
	void Cleanup();
private:
	FMOD::System* fmodSystem;
	const int maxChannels = 512;
	std::vector<AudioClip> audioClips;
};

