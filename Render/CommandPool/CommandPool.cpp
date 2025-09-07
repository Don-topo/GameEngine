#include "CommandPool.h"

void CommandPool::Initialization(VkDevice& device, vkb::QueueType queueType)
{
	VkCommandPoolCreateInfo commandPoolCreateInfo = {};
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	// TODO Need to store the command pool index somewhere
	//commandPoolCreateInfo.queueFamilyIndex = 
	commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	//DEV_ASSERT(vkCreateCommandPool(device, &commandPoolCreateInfo, nullptr, &commandPool), "CommandPool", "Error creating the Command Pool!");
}

void CommandPool::Cleanup(VkDevice& device)
{
	//vkDestroyCommandPool(device, commandPool, nullptr);
}
