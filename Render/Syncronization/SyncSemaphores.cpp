#include "SyncSemaphores.h"

void SyncSemaphores::Initialization(VkDevice& device)
{
	VkSemaphoreCreateInfo semaphoreCreateInfo = {};
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	
	DEV_ASSERT(vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &graphicsSemaphore) == VK_SUCCESS, "SyncSemaphores", "Failed to create Graphics Semaphore!");
	DEV_ASSERT(vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &computeSemaphore) == VK_SUCCESS, "SyncSemaphores", "Failed to create Compute Semaphore!");
	DEV_ASSERT(vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &presentSemaphore) == VK_SUCCESS, "SyncSemaphores", "Failed to create Present Sempahore!");
	DEV_ASSERT(vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &renderSemaphore) == VK_SUCCESS, "SyncSemaphores", "Failed to create Render Semaphore!");
	DEV_LOG(TE_INFO, "SyncSemaphores", "Semaphores created!");
}

void SyncSemaphores::Cleanup(VkDevice& device)
{
	vkDestroySemaphore(device, graphicsSemaphore, nullptr);
	vkDestroySemaphore(device, computeSemaphore, nullptr);
	vkDestroySemaphore(device, presentSemaphore, nullptr);
	vkDestroySemaphore(device, renderSemaphore, nullptr);
	DEV_LOG(TE_INFO, "SyncSemaphores", "Semaphores destroyed!");
}