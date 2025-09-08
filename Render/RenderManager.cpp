#include "RenderManager.h"

void RenderManager::Initialization()
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
	// Create device
	// TODO Select best available device

	DEV_LOG(TE_INFO, "RenderManager", "Physical device selected");

	// Create Swapchain
	// Create CommandPool
	// Create CommandBuffer

	// Create queues (Graphics, compute and present)
	// Create Pipeline
	// Create Framebuffer
}

void RenderManager::Cleanup()
{

}