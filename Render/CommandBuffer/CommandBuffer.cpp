#include "CommandBuffer.h"

void CommandBuffer::Initialization(VkDevice device, VkCommandPool commandPool)
{
	this->commandPool = commandPool;

	VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.commandPool = commandPool;
	commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	commandBufferAllocateInfo.commandBufferCount = 1;

	DEV_ASSERT(vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, &commandBuffer), "CommandBuffer", "Error allocating the CommandBuffer!");
}

void CommandBuffer::Begin(VkCommandBufferBeginInfo& commandBufferBeginInfo)
{
	DEV_ASSERT(vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo), "CommandBuffer", "Error begining CommandBuffer!");
}

void CommandBuffer::End()
{
	DEV_ASSERT(vkEndCommandBuffer(commandBuffer), "CommandBuffer", "Error ending the CommandBuffer!");
}

void CommandBuffer::Reset(VkCommandBufferResetFlags resetFlags)
{
	DEV_ASSERT(vkResetCommandBuffer(commandBuffer, resetFlags), "CommandBuffer", "Error reseting the CommandBuffer");
}

void CommandBuffer::Cleanup(VkDevice device)
{
	vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
	DEV_LOG(TE_INFO, "CommandBuffer", "CommandBuffer destroyed!");
}