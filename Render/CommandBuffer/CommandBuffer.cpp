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

void CommandBuffer::BeginSingleShot()
{
	VkCommandBufferBeginInfo commandBufferBeginInfo = {};
	commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	DEV_ASSERT(vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo), "CommandBuffer", "Error creating the one shot CommandBuffer!");
}

void CommandBuffer::CreateSingleShotBuffer(VkDevice device, VkCommandPool commandPool)
{
	Initialization(device, commandPool);

	Reset();

	BeginSingleShot();
}

void CommandBuffer::submitSingleShotBuffer(VkDevice device, VkCommandPool commandPool, VkQueue queue)
{
	End();

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	VkFence fence;

	VkFenceCreateInfo fenceCreateInfo = {};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	DEV_ASSERT(vkCreateFence(device, &fenceCreateInfo, nullptr, &fence), "CommandBuffer", "Error creating the fence in SubmitSingleShotBuffer!");

	DEV_ASSERT(vkResetFences(device, 1, &fence), "CommandBuffer", "Error reseting the Fence in SubmitSingleShotBuffer!");

	DEV_ASSERT(vkQueueSubmit(queue, 1, &submitInfo, fence), "CommandBuffer", "Error submiting the Queue in SubmitSingleShotBuffer!");

	DEV_ASSERT(vkWaitForFences(device, 1, &fence, VK_TRUE, UINT32_MAX), "CommandBuffer", "Error waiting the Fence in SubmitSingleShotBuffer!");

	vkDestroyFence(device, fence, nullptr);

	Cleanup(device);
}

void CommandBuffer::Cleanup(VkDevice device)
{
	vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
	DEV_LOG(TE_INFO, "CommandBuffer", "CommandBuffer destroyed!");
}