#include "CommandPool.h"

void CommandPool::Initialization(vkb::Device& device, vkb::QueueType queueType)
{
	VkCommandPoolCreateInfo commandPoolCreateInfo {};
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;	
	commandPoolCreateInfo.queueFamilyIndex = device.get_queue_index(queueType).value();	
	commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	DEV_ASSERT(vkCreateCommandPool(device.device, &commandPoolCreateInfo, nullptr, &commandPool) == VK_SUCCESS, "CommandPool", "Error creating the Command Pool!");
	DEV_LOG(TE_INFO, "CommandPool", "CommandPool created!");
}

void CommandPool::Cleanup(VkDevice& device)
{
	vkDestroyCommandPool(device, commandPool, nullptr);
	DEV_LOG(TE_INFO, "CommandPool", "CommandPool destroyed!");
}
