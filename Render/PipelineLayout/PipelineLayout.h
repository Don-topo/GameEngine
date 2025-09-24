#pragma once

#include "../../Logger/Logger.h"
#include <vulkan/vulkan.h>
#include <vector>

class PipelineLayout
{
public:
	void Initialization(VkDevice device, std::vector<VkDescriptorSetLayout> descriptorLayouts, std::vector<VkPushConstantRange> pushConstants = {});
	VkPipelineLayout GetPipelineLayout() { return pipelineLayout; }
	void Cleanup(VkDevice device);

private:
	VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
};