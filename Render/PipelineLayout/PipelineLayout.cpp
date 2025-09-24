#include "PipelineLayout.h"

void PipelineLayout::Initialization(VkDevice device, VkDescriptorSetLayout descriptorLayouts[], VkPushConstantRange pushConstants[])
{
	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(sizeof(descriptorLayouts));
	pipelineLayoutInfo.pSetLayouts = descriptorLayouts;
	pipelineLayoutInfo.pushConstantRangeCount = static_cast<uint32_t>(sizeof(pushConstants));
	pipelineLayoutInfo.pPushConstantRanges = pushConstants;

	DEV_ASSERT(vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) == VK_SUCCESS, "PipelineLayout", "Error creating the PipelineLayout!");
	DEV_LOG(TE_INFO, "PipelineLayout", "PipelineLayout created!");
}

void PipelineLayout::Cleanup(VkDevice device)
{
	if (pipelineLayout != nullptr)
	{
		vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
		DEV_LOG(TE_INFO, "PipelineLayout", "PipelineLayout deleted!");
		return;
	}
	DEV_LOG(TE_WARNING, "PipelineLayout", "Pipeline already deleted!");
}