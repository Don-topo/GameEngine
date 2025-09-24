#pragma once

#include <vulkan/vulkan.h>
#include "../../Logger/Logger.h"
#include <vector>

class RenderPass
{
public:
	// TODO Need device, VkFormat (image + depth) 
	void Initialization(VkDevice device, VkFormat imageFormat, VkFormat deptFormat);
	VkRenderPass GetRenderPass() { return renderPass; }
	void Cleanup(VkDevice device);

private:
	VkRenderPass renderPass;
};