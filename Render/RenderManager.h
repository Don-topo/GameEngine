#pragma once
#include <VkBootstrap.h>
#include "vulkan/vulkan.h"
#include "vk_mem_alloc.h"

class RenderManager
{
public:
	//RenderManager();
	//~RenderManager();
	void Initialization();
private:
	VkInstance instance;
};
