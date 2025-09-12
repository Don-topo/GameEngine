#pragma once

#include "../Logger/Logger.h"
#include <VkBootstrap.h>
#include "vulkan/vulkan.h"
#include <vma/vk_mem_alloc.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <glm/glm.hpp>
#include <vector>
#include "CommandPool/CommandPool.h"

class RenderManager
{
public:
	//RenderManager();
	//~RenderManager();
	void Initialization(SDL_Window* window);
	void Update();
	void Cleanup();
private:
	vkb::Instance instance;
	VkSurfaceKHR surface;
	vkb::PhysicalDevice physicalDevice;
	vkb::Device device;
	VmaAllocator allocator;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	vkb::Swapchain swapchain;
	std::vector<VkImage> swapchainImages;
	std::vector<VkImageView> swapchainImageViews;
	CommandPool graphicsCommandPool;
	CommandPool computeCommandPool;
};
