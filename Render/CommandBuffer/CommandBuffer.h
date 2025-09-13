#pragma once

#include <memory>
#include <vulkan/vulkan.h>
#include "../../Logger/Logger.h"

class CommandBuffer
{
public:
	void Initialization(VkDevice device, VkCommandPool commandPool);
	void Begin(VkCommandBufferBeginInfo& commandBufferBeginInfo);
	void End();
	void Reset(VkCommandBufferResetFlags resetFlags = 0);
	VkCommandBuffer GetCommandBuffer() { return commandBuffer; }
	void Cleanup(VkDevice device);
private:
	VkCommandBuffer commandBuffer;
	VkCommandPool commandPool;
};
