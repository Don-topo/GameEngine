#pragma once

#include "../Logger/Logger.h"
#include <VkBootstrap.h>
#include "vulkan/vulkan.h"
#include "vk_mem_alloc.h"

class RenderManager
{
public:
	//RenderManager();
	//~RenderManager();
	void Initialization();
	void Update();
	void Cleanup();
private:
	VkInstance instance;
};
