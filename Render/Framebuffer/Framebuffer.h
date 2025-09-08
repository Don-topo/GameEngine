#pragma once

#include "../../Logger/Logger.h"
#include <vulkan/vulkan.h>
#include <vector>

class Framebuffer
{
public:
	//Framebuffer();
	//~Framebuffer();
	void Initialization(VkDevice device);
	void Cleanup(VkDevice device);
private:
	//std::vector<
};