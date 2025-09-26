#pragma once

#include "../../Logger/Logger.h"
#include <vulkan/vulkan.h>
#include <vector>
#include <VkBootstrap.h>

class Framebuffer
{
public:
	void Initialization(VkDevice device, VkRenderPass renderPass, VkImageView imageView, vkb::Swapchain swapchain);
	std::vector<VkFramebuffer> GetFrameBuffers() { return framebuffers; }
	void Cleanup(VkDevice device);
private:
	std::vector<VkFramebuffer> framebuffers;
};