#include "Framebuffer.h"

void Framebuffer::Initialization(VkDevice device, VkRenderPass renderPass, VkImageView imageView, vkb::Swapchain swapchain)
{
	unsigned int swapchainImageViewSize = swapchain.get_image_views().value().size();

	// Resize framebuffers to match the swapchain imageViews
	framebuffers.resize(swapchainImageViewSize);

	// Create a framebuffer for every imageView
	for (unsigned int i = 0; i < swapchainImageViewSize; i++)
	{
		std::vector<VkImageView> imagesToAttach = { swapchain.get_image_views().value().at(i), imageView };

		VkFramebufferCreateInfo framebufferCreateInfo = {};
		framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;		
		framebufferCreateInfo.renderPass = renderPass;
		framebufferCreateInfo.pAttachments = imagesToAttach.data();
		framebufferCreateInfo.attachmentCount = static_cast<uint32_t>(imagesToAttach.size());
		framebufferCreateInfo.width = swapchain.extent.width;
		framebufferCreateInfo.height = swapchain.extent.height;
		framebufferCreateInfo.layers = 1;

		DEV_ASSERT(vkCreateFramebuffer(device, &framebufferCreateInfo, nullptr, &framebuffers.at(i)) == VK_SUCCESS, "Framebuffer", "Failed to create the Framebuffer!");
	}
	DEV_LOG(TE_INFO, "Framebuffer", "Framebuffers created!");
}

void Framebuffer::Cleanup(VkDevice device) {
	for each(VkFramebuffer framebuffer in framebuffers)
	{
		vkDestroyFramebuffer(device, framebuffer, nullptr);
	}
	DEV_LOG(TE_INFO, "Framebuffer", "Framebuffers destroyed!");
}