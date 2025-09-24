#include "PipelineLayout.h"

void PipelineLayout::Initialization(VkDevice device, std::vector<VkDescriptorSetLayout> descriptorLayouts, std::vector<VkPushConstantRange> pushConstants)
{
	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorLayouts.size());
	pipelineLayoutInfo.pSetLayouts = descriptorLayouts.data();
	if (pushConstants.size() > 0)
	{
		pipelineLayoutInfo.pushConstantRangeCount = static_cast<uint32_t>(pushConstants.size());
		pipelineLayoutInfo.pPushConstantRanges = pushConstants.data();
	}	

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