#include "RenderManager.h"

void RenderManager::Initialization(SDL_Window* window)
{
	// Create vulkan instance with vk-bootstrap
	vkb::InstanceBuilder builder;
	vkb::Result<vkb::Instance> bInstance = builder.
		use_default_debug_messenger().
		request_validation_layers().
		require_api_version(1, 1, 0).
		build();

	DEV_ASSERT(bInstance.has_value(), "RenderManager", "Error creating Vulkan instance!");
	instance = bInstance.value();
	DEV_LOG(TE_INFO, "RenderManager", "Instance created!");

	// Create surface
	DEV_ASSERT(SDL_Vulkan_LoadLibrary(NULL), "RenderManager", "Failed to load Vulkan library!");
	VkSurfaceKHR surface; 
	SDL_Vulkan_CreateSurface(window, instance, NULL, &surface);
	DEV_LOG(TE_INFO, "RenderManager", "Surface created!");
	// Create device
	VkPhysicalDeviceFeatures requiredFeatures{};
	requiredFeatures.samplerAnisotropy = VK_TRUE;
	vkb::PhysicalDeviceSelector physicalDeviceSelector{ instance };
	vkb::Result<vkb::PhysicalDevice> returnPhysicalDeviceSelector = physicalDeviceSelector.
		set_surface(surface).
		set_required_features(requiredFeatures).
		select();
	DEV_ASSERT(returnPhysicalDeviceSelector.has_value(), "RenderManager", "Failed to obtain a physical device!");
	// TODO Select best available device
	// TODO Create Vkdevice (physical and logic)

	DEV_LOG(TE_INFO, "RenderManager", "Physical device selected");

	// Create Swapchain
	// Create CommandPool
	// Create CommandBuffer

	// Create queues (Graphics, compute and present)
	// Create Pipeline NOTE Maybe need multiple pipelines to avoid creating then in real time
	// Create Framebuffer

	// TODO Create semaphores to sync GPU and CPU (fences)
}

void RenderManager::Cleanup()
{

}