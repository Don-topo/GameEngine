#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "../../Logger/Logger.h"

class SecondRenderPass
{
public:
	void Initialize(VkDevice& device, VkFormat& imageFormat, VkFormat& depthFormat);
	VkRenderPass& GetRenderPass() { return renderPass; }
	void Cleanup(VkDevice& device);
private:
	VkRenderPass renderPass;
};