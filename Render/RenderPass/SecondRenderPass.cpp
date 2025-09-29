#include "SecondRenderPass.h"

void SecondRenderPass::Initialize(VkDevice device, VkFormat imageFormat, VkFormat depthFormat)
{
	VkAttachmentDescription colorAttachmentDescription = {};
	colorAttachmentDescription.format = imageFormat;
	colorAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
	colorAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	colorAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentReference = {};
	colorAttachmentReference.attachment = 0;
	colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentDescription depthAttachmentDecription = {};
	depthAttachmentDecription.flags = 0;
	depthAttachmentDecription.format = depthFormat;
	depthAttachmentDecription.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachmentDecription.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
	depthAttachmentDecription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	depthAttachmentDecription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachmentDecription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachmentDecription.initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	depthAttachmentDecription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentReference = {};
	depthAttachmentReference.attachment = 1;
	depthAttachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpassDesc = {};
	subpassDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpassDesc.colorAttachmentCount = 1;
	subpassDesc.pColorAttachments = &colorAttachmentReference;
	subpassDesc.pDepthStencilAttachment = &depthAttachmentReference;

	VkSubpassDependency colorSubpassDependency = {};
	colorSubpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	colorSubpassDependency.dstSubpass = 0;
	colorSubpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	colorSubpassDependency.srcAccessMask = 0;
	colorSubpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	colorSubpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkSubpassDependency depthSubpassDependency = {};
	depthSubpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	depthSubpassDependency.dstSubpass = 0;
	depthSubpassDependency.srcStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
	depthSubpassDependency.srcAccessMask = 0;
	depthSubpassDependency.dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
	depthSubpassDependency.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

	std::vector<VkSubpassDependency> subpassDependencies = { colorSubpassDependency, depthSubpassDependency };
	std::vector<VkAttachmentDescription> attachmentsDescriptions = { colorAttachmentDescription, depthAttachmentDecription };

	VkRenderPassCreateInfo renderPassCreateInfo = {};
	renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassCreateInfo.attachmentCount = static_cast<uint32_t>(attachmentsDescriptions.size());
	renderPassCreateInfo.pAttachments = attachmentsDescriptions.data();
	renderPassCreateInfo.subpassCount = 1;
	renderPassCreateInfo.pSubpasses = &subpassDesc;
	renderPassCreateInfo.dependencyCount = static_cast<uint32_t>(subpassDependencies.size());
	renderPassCreateInfo.pDependencies = subpassDependencies.data();

	DEV_ASSERT(vkCreateRenderPass(device, &renderPassCreateInfo, nullptr, &renderPass), "SecondRenderPass", "Error creating RenderPass!");
}

void SecondRenderPass::Cleanup(VkDevice device)
{
	vkDestroyRenderPass(device, renderPass, nullptr);
	DEV_LOG(TE_INFO, "SecondRenderPass", "RenderPass destroyed!");
}