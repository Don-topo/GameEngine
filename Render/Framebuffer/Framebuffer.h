#pragma once

#include "../../Logger/Logger.h"
#include <vulkan/vulkan.h>
#include <vector>
#include <VkBootstrap.h>

class Framebuffer
{
public:
	void Initialization(VkDevice device, vkb::Swapchain swapchain);
	void Cleanup(VkDevice device);
private:
	std::vector<VkFramebuffer> framebuffers;
};