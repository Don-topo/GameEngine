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

	/* required for dynamic buffer with world position matrices */
	DEV_LOG(TE_INFO, "RenderManager", "Physical device selected");
	VkDeviceSize offsetAligment = physicalDevice.properties.limits.minStorageBufferOffsetAlignment;
	offsetAligment = std::max(offsetAligment, sizeof(glm::mat4));
	vkb::DeviceBuilder deviceBuilder { physicalDevice };
	vkb::Result<vkb::Device> deviceBuilderReturn = deviceBuilder.build();
	DEV_ASSERT(deviceBuilderReturn.has_value(), "RenderManager", "Error creating the logic device!");
	device = deviceBuilderReturn.value();
	DEV_LOG(TE_INFO, "RenderManager", "Device created!");

	// Init VMA
	VmaAllocatorCreateInfo allocatorInfo{};
	allocatorInfo.physicalDevice = physicalDevice;
	allocatorInfo.device = device.device;
	allocatorInfo.instance = instance.instance;

	DEV_ASSERT(vmaCreateAllocator(&allocatorInfo, &allocator) == VK_SUCCESS, "RenderManager", "Failed to initialize VMA!");	

	// Create VK_QUEUES (Present + Compute)
	vkb::Result<VkQueue> graphicsQueueResult = device.get_queue(vkb::QueueType::graphics);
	DEV_ASSERT(graphicsQueueResult.has_value(), "RenderManager", "Failed to get Graphics queue result!");
	graphicsQueue = graphicsQueueResult.value();

	vkb::Result<VkQueue> presentQueueResult = device.get_queue(vkb::QueueType::present);
	DEV_ASSERT(presentQueueResult.has_value(), "RenderManager", "Failed to get Present queue result!");
	presentQueue = presentQueueResult.value();

	DEV_LOG(TE_INFO, "RenderManager", "Queues created!");


	// Create Swapchain
	vkb::SwapchainBuilder swapChainBuilder{ device };
	VkSurfaceFormatKHR surfaceFormat;

	/* set surface to non-sRGB */
	surfaceFormat.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	surfaceFormat.format = VK_FORMAT_B8G8R8A8_UNORM;

	/* VK_PRESENT_MODE_FIFO_KHR enables vsync */
	vkb::Result<vkb::Swapchain> swapChainBuildReturn = swapChainBuilder
		.set_old_swapchain(swapchain)
		.set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
		.set_desired_format(surfaceFormat)
		.build();

	DEV_ASSERT(swapChainBuildReturn.has_value(), "RenderManager", "Failed to create the Swapchain!");

	vkb::destroy_swapchain(swapchain);
	swapchain = swapChainBuildReturn.value();
	swapchainImages = swapChainBuildReturn.value().get_images().value();
	swapchainImageViews = swapChainBuildReturn.value().get_image_views().value();

	DEV_LOG(TE_INFO, "RenderManager", "Swapchain created!");

	// Create CommandPool (Graphics + Compute)
	graphicsCommandPool.Initialization(device, vkb::QueueType::graphics);
	computeCommandPool.Initialization(device, vkb::QueueType::compute);

	// Create CommandBuffer
	commandBuffer.Initialization(device, graphicsCommandPool.GetCommandPool());
	
	// Create Pipeline NOTE Maybe need multiple pipelines to avoid creating then in real time
	// Create Framebuffer
	//framebuffer.Initialization(device, swapchain);

	// TODO Create semaphores to sync GPU and CPU (fences)
	fences.Initialization(device);
	semaphores.Initialization(device);
	// TODO DepthBuffer
	// TODO VertexBuffer

	// TODO Create matrix to apply translations and rotations
	// TODO Descriptors
		// TODO Descriptor pool
		// TODO Descriptor layouts
		// TODO Descriptor sets
	// TODO RenderPass
	// TODO Pipeline layout
	// Skybox
	VkDescriptorSetLayout rdAssimpTextureDescriptorLayout = VK_NULL_HANDLE;
	VkDescriptorSetLayout rdSkyboxDescriptorLayout = VK_NULL_HANDLE;
	VkDescriptorSetLayout skyboxLayouts[] =
	{
		rdAssimpTextureDescriptorLayout,
		rdSkyboxDescriptorLayout
	};
	//skyboxLayout.Initialization(device, skyboxLayouts);
	// TODO Pipeline
	// Skybox
	std::string vertexShaderFileName = "skybox.vert.spv";
	std::string fragmentShaderFileName = "skybox.frag.spv";
	//skyboxPipeline.Initialization(device, skyboxLayout.GetPipelineLayout(), renderpass, vertexShaderFileName, fragmentShaderFileName);

}

void RenderManager::Cleanup()
{	
	framebuffer.Cleanup(device);
	commandBuffer.Cleanup(device);
	graphicsCommandPool.Cleanup(device.device);
	computeCommandPool.Cleanup(device.device);
	swapchain.destroy_image_views(swapchainImageViews);
	DEV_LOG(TE_INFO, "RenderManager", "Swapchain Image Views destroyed!");
	vkb::destroy_swapchain(swapchain);
	DEV_LOG(TE_INFO, "RenderManager", "Swapchain destroyed!");
	vkb::destroy_device(device);
	DEV_LOG(TE_INFO, "RenderManager", "Device destroyed!");
	vkb::destroy_surface(instance, surface);
	DEV_LOG(TE_INFO, "RenderManager", "Surface destroyed!");
	vkb::destroy_instance(instance);	
	DEV_LOG(TE_INFO, "RenderManager", "Instance destroyed!");
}