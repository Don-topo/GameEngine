#pragma once
#include "../../Logger/Logger.h"
#include "VkBootstrap.h"
#include <vulkan/vulkan.h>
#include <VkBootstrap.h>

class CommandPool
{
public:
	void Initialization(vkb::Device device, vkb::QueueType queueType);
	VkCommandPool GetCommandPool() { return commandPool; }
	void Cleanup(VkDevice& device);

private:
	VkCommandPool commandPool;
};