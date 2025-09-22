#pragma once

#include "../../Logger/Logger.h"
#include <vulkan/vulkan.h>

class PipelineLayout
{
public:
	void Initialization(VkDevice device, VkDescriptorSetLayout descriptorLayouts[], VkPushConstantRange pushConstants[] = {});
	VkPipelineLayout GetPipelineLayout() { return pipelineLayout; }
	void Cleanup(VkDevice device);

private:
	VkPipelineLayout pipelineLayout;
};