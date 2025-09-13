#pragma once

#include <memory>
#include <vulkan/vulkan.h>
#include "../../Logger/Logger.h"

class CommandBuffer
{
public:
	void Initialization(VkDevice device);
	VkCommandBuffer GetCommandBuffer() { return commandBuffer; }
	void Cleanup(VkDevice device);
private:
	VkCommandBuffer commandBuffer;
};
