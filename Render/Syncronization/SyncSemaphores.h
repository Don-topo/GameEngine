#pragma once

#include <vulkan/vulkan.h>
#include "../../Logger/Logger.h"

class SyncSemaphores
{
public:
	void Initialization(VkDevice device);
	void Cleanup(VkDevice device);

	VkSemaphore& GetGraphicsSemaphore() { return graphicsSemaphore; }
	VkSemaphore& GetComputeSemaphore() { return computeSemaphore; }
	VkSemaphore& GetPresentSemaphore() { return presentSemaphore; }
	VkSemaphore& GetRenderSemaphore() { return renderSemaphore; }

private:
	VkSemaphore graphicsSemaphore;
	VkSemaphore computeSemaphore;
	VkSemaphore presentSemaphore;
	VkSemaphore renderSemaphore;
};
