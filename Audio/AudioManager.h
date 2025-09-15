#pragma once

#include <fmod.hpp>
#include <string>
#include <vector>
#include "../Logger/Logger.h"

class AudioManager
{
public:
	void Initialization();
	void Update();
	void Cleanup();
private:
	FMOD::System* fmodSystem;
	const int maxChannels = 512;
};

