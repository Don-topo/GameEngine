#include "SyncFences.h"

void SyncFences::Initialization(VkDevice& device)
{
	VkFenceCreateInfo fenceCreateInfo = {};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

	DEV_ASSERT(vkCreateFence(device, &fenceCreateInfo, nullptr, &renderFence) == VK_SUCCESS, "SyncFences", "Failed to create Render Fence!");
	DEV_ASSERT(vkCreateFence(device, &fenceCreateInfo, nullptr, &computeFence) == VK_SUCCESS, "SyncFences", "Failed to create Compute Fence!");
	DEV_LOG(TE_INFO, "SyncFences", "Fences Created!");
}

void SyncFences::Cleanup(VkDevice& device)
{
	vkDestroyFence(device, renderFence, nullptr);
	vkDestroyFence(device, computeFence, nullptr);
	DEV_LOG(TE_INFO, "SyncFences", "Fences destroyed!");
}