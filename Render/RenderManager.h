#pragma once

#include "../Logger/Logger.h"
#include <VkBootstrap.h>
#include "vulkan/vulkan.h"
#include "vk_mem_alloc.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

class RenderManager
{
public:
	//RenderManager();
	//~RenderManager();
	void Initialization(SDL_Window* window);
	void Update();
	void Cleanup();
private:
	vkb::Instance instance;
};
