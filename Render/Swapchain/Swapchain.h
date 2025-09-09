#pragma once

#include "../../Logger/Logger.h"
#include "vulkan/vulkan.h"
#include "VkBootstrap.h"

class Swapchain
{
public:
	Swapchain(VkDevice device);
	~Swapchain();
	void Initialization(VkDevice Device);
	void ResizeSwapchain();
	void GetSwapchain();
	void Cleanup();
private:
	vkb::Swapchain swapchain;
};