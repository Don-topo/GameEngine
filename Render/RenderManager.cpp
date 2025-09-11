#include "RenderManager.h"

void RenderManager::Initialization(SDL_Window* window)
{
	DEV_ASSERT(SDL_Vulkan_LoadLibrary(NULL), "RenderManager", "Failed to load Vulkan library!");

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
	SDL_Vulkan_CreateSurface(window, instance, NULL, &surface);
	DEV_LOG(TE_INFO, "RenderManager", "Surface created!");

	// Select best available device
	VkPhysicalDeviceFeatures requiredFeatures{};
	requiredFeatures.samplerAnisotropy = VK_TRUE;
	vkb::PhysicalDeviceSelector physicalDeviceSelector{ instance };
	vkb::Result<vkb::PhysicalDevice> returnPhysicalDeviceSelector = physicalDeviceSelector.
		set_surface(surface).
		set_required_features(requiredFeatures).
		select();
	DEV_ASSERT(returnPhysicalDeviceSelector.has_value(), "RenderManager", "Failed to obtain a physical device!");

	VkPhysicalDeviceFeatures requiredFeatures2{};

	vkGetPhysicalDeviceFeatures(returnPhysicalDeviceSelector.value(), &requiredFeatures2);
	vkb::Result<vkb::PhysicalDevice> mainReturnedPhysicalDevice = physicalDeviceSelector.
		set_surface(surface).
		set_required_features(requiredFeatures2).
		select();

	DEV_ASSERT(mainReturnedPhysicalDevice.has_value(), "RenderManager", "Failed to obtain a suitable physical device!");

	// Physical device
	physicalDevice = mainReturnedPhysicalDevice.value();

	DEV_LOG(TE_INFO, "RenderManager", "Physical device selected");

	/* required for dynamic buffer with world position matrices */
	/*VkDeviceSize minSSBOOffsetAlignment = mRenderData.rdVkbPhysicalDevice.properties.limits.minStorageBufferOffsetAlignment;
	Logger::log(1, "%s: the physical device has a minimal SSBO offset of %i bytes\n", __FUNCTION__, minSSBOOffsetAlignment);
	mMinSSBOOffsetAlignment = std::max(minSSBOOffsetAlignment, sizeof(glm::mat4));
	Logger::log(1, "%s: SSBO offset has been adjusted to %i bytes\n", __FUNCTION__, mMinSSBOOffsetAlignment);

	vkb::DeviceBuilder devBuilder{ mRenderData.rdVkbPhysicalDevice };
	auto devBuilderRet = devBuilder.build();
	if (!devBuilderRet) {
		Logger::log(1, "%s error: could not get devices\n", __FUNCTION__);
		return false;
	}
	mRenderData.rdVkbDevice = devBuilderRet.value();*/

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
	vkb::destroy_surface(instance, surface);
	DEV_LOG(TE_INFO, "RenderManager", "Surface destroyed!");
	vkb::destroy_instance(instance);	
	DEV_LOG(TE_INFO, "RenderManager", "Instance destroyed!");
}