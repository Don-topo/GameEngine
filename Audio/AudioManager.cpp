#include "AudioManager.h"

void AudioManager::Initialization()
{
	DEV_ASSERT(FMOD::System_Create(&fmodSystem), "AudioManager", "Error creating the FMOD System!");
	DEV_ASSERT(fmodSystem->init(maxChannels, FMOD_INIT_NORMAL, nullptr), "AudioManager", "Error initializating the audio channels!");
	DEV_ASSERT(fmodSystem->set3DSettings(1.0f, 1.0f, 1.0f), "AudioManager", "Error setting 3D Audio!");
	DEV_LOG(TE_INFO, "AudioManager", "FMOD Initialized!");
}

void AudioManager::Update()
{
	if (fmodSystem)
	{
		fmodSystem->update();
	}
}

void AudioManager::Cleanup()
{
	if (fmodSystem != nullptr)
	{
		DEV_ASSERT(fmodSystem->release(), "AudioManager", "Error releasing FMOD!");
		DEV_ASSERT(fmodSystem->close(), "AudioManager", "Error closing FMOD!");
		fmodSystem = nullptr;
		DEV_LOG(TE_INFO, "AudioManager", "FMOD Destroyed!");
	}
	else
	{
		DEV_LOG(TE_WARNING, "AudioManager", "FMOD is already closed!");
	}
	
}