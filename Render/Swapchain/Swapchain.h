#pragma once

#include "../../Logger/Logger.h"
#include "vulkan/vulkan.h"
#include "VkBootstrap.h"

class Swapchain
{
public:
	void Initialization(vkb::Device& device);
	void ResizeSwapchain();
	vkb::Swapchain& GetSwapchain() { return swapchain; }
	void Cleanup();
private:
	vkb::Swapchain swapchain;
};