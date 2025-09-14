#include "RenderPass.h"

void RenderPass::Initialization(VkDevice device, VkFormat imageFormat, VkFormat depthFormat)
{
	VkAttachmentDescription colAttachment = {};
}

void RenderPass::Cleanup(VkDevice device)
{
	vkDestroyRenderPass(device, renderPass, nullptr);
	DEV_LOG(TE_INFO, "RenderPass", "RenderPass destroyed!");
}