#pragma once
#include <memory>
#include <string>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include "../Logger/Logger.h"
#include "../Render/RenderManager.h"

class GameEngine
{
public:
	//GameEngine();
	//~GameEngine();
	void Initialization(unsigned int width, unsigned int height, std::string title);
	void Loop();
	void Cleanup();

private:
	SDL_Window* sdlWindow = nullptr;
	std::string title;
	std::unique_ptr<RenderManager> renderManager = nullptr;
};