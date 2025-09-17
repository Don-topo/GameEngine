#include "AudioManager.h"

void AudioManager::Initialization()
{
	DEV_ASSERT(FMOD::System_Create(&fmodSystem) == FMOD_OK, "AudioManager", "Error creating the FMOD System!");	
	DEV_ASSERT(fmodSystem->init(maxChannels, FMOD_INIT_NORMAL, nullptr) == FMOD_OK, "AudioManager", "Error initializating the audio channels!");
	DEV_ASSERT(fmodSystem->set3DSettings(1.0f, 1.0f, 1.0f) == FMOD_OK, "AudioManager", "Error setting 3D Audio!");
	DEV_LOG(TE_INFO, "AudioManager", "FMOD Initialized!");

	//SetAudioListener(glm::vec3{}, glm::vec3{}, glm::vec3{ 0.0f, 0.0f, 1.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f });
}

void AudioManager::LoadAudio(std::string fileName)
{
	FMOD::Sound* newSound = nullptr;
	DEV_ASSERT(fmodSystem->createSound(fileName.c_str(), FMOD_DEFAULT, nullptr, &newSound) == FMOD_OK, "AudioManager", "Error loading audio clip");
	DEV_LOG(TE_INFO, "AudioManager", "Audio file loaded!");
	AudioClip newAudioClip;	
	newAudioClip.Initialization(fileName, newSound);
	audioClips.push_back(newAudioClip);
}

void AudioManager::PlayAudio(std::string fileName)
{
	// Safety check to avoid errors
	if (fmodSystem != nullptr)
	{
		for (std::vector<AudioClip>::iterator it = audioClips.begin(); it < audioClips.end(); it++)
		{
			// Check if we found the audioClip (NOTE: Must be loaded
			if (it->GetFileName() == fileName)
			{
				FMOD::Channel* channel = nullptr;
				DEV_ASSERT(fmodSystem->playSound(it->GetSound(), nullptr, true, &channel) == FMOD_OK, "AudioManager", "Error Playing the audio clip");
				it->SetChannel(channel);
				return;
			}			
		}
		DEV_LOG(TE_WARNING, "AudioManager", "AudioClip not loaded!");
	}
}

void AudioManager::PauseOrResumeAudio(std::string fileName)
{
	if (fmodSystem != nullptr)
	{
		for (std::vector<AudioClip>::iterator it = audioClips.begin(); it != audioClips.end(); it++)
		{
			if (it->GetFileName() == fileName)
			{
				bool isPaused;
				it->GetChannel()->getPaused(&isPaused);
				it->GetChannel()->setPaused(!isPaused);
				return;
			}
		}
		DEV_LOG(TE_WARNING, "AudioManager", "Warning AudioClip not found!");
	}
}

void AudioManager::SetVolume(std::string fileName, float volume)
{
	if (fmodSystem != nullptr)
	{
		for (std::vector<AudioClip>::iterator it = audioClips.begin(); it != audioClips.end(); it++)
		{
			if (it->GetFileName() == fileName && it->GetChannel())
			{
				DEV_ASSERT(it->GetChannel()->setVolume(volume) == FMOD_OK, "AudioManager", "Error setting volume!");
				return;
			}
		}
	}
}

void AudioManager::RemoveAudioClip(std::string fileName)
{
	for (std::vector<AudioClip>::iterator it = audioClips.begin(); it != audioClips.end(); it++)
	{
		if (it->GetFileName() == fileName)
		{
			// Stop the audio to avoid errors
			it->Cleanup();
			audioClips.erase(it);
			// Resize vector to avoid errors on others methods
			audioClips.resize(audioClips.size() - 1);
			DEV_LOG(TE_INFO, "AudioManager", "AudioClip removed!");
			return;
		}
	}
	DEV_LOG(TE_WARNING, "AudioManager", "AudioClip not found!");
}

void AudioManager::SetAudioListener(glm::vec3 pos, glm::vec3 vel, glm::vec3 frwd, glm::vec3 up)
{
	if (fmodSystem != nullptr)
	{
		FMOD_VECTOR position = { pos.x, pos.y, pos.z };
		FMOD_VECTOR velocity = { vel.x, vel.y, vel.z };
		FMOD_VECTOR forward = { frwd.x, frwd.y, frwd.z };
		FMOD_VECTOR _up = { up.x, up.y, up.z };

		DEV_ASSERT(fmodSystem->set3DListenerAttributes(0, &position, &velocity, &forward, &_up) == FMOD_OK, "AudioManager", "Error Setting the audio listener!");
		DEV_LOG(TE_VERBOSE, "AudioManager", "Listener setted!");
	}
}

void AudioManager::Update()
{
	if (fmodSystem != nullptr)
	{
		fmodSystem->update();
	}
}

void AudioManager::Cleanup()
{
	// Clean AudioClips
	audioClips.clear();

	// Stop and Clean FMOD
	if (fmodSystem != nullptr)
	{		
		DEV_ASSERT(fmodSystem->close() == FMOD_OK, "AudioManager", "Error closing FMOD!");
		DEV_ASSERT(fmodSystem->release() == FMOD_OK, "AudioManager", "Error releasing FMOD!");		
		fmodSystem = nullptr;
		DEV_LOG(TE_INFO, "AudioManager", "FMOD Destroyed!");
	}
	else
	{
		DEV_LOG(TE_WARNING, "AudioManager", "FMOD is already closed!");
	}
	
}