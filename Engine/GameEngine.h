#pragma once
#include <memory>
#include <string>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include "../Logger/Logger.h"
#include "../Render/RenderManager.h"
#include "../Audio/AudioManager.h"

class GameEngine
{
public:	
	void Initialization(unsigned int width, unsigned int height, std::string title);
	void Loop();
	void Cleanup();

private:
	SDL_Window* sdlWindow = nullptr;
	std::string title;
	std::unique_ptr<RenderManager> renderManager = nullptr;
	std::unique_ptr<AudioManager> audioManager = nullptr;
};