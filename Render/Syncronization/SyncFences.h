#pragma once

#include <vulkan/vulkan.h>
#include "../../Logger/Logger.h"

class SyncFences
{
public:
	void Initialization(VkDevice& device);
	void Cleanup(VkDevice& device);

	VkFence& GetRenderFence() { return renderFence; }
	VkFence& GetComputeFence() { return computeFence; }

private:
	VkFence renderFence;
	VkFence computeFence;
};