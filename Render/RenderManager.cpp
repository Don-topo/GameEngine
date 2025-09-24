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

	vkb::Result<VkQueue> computeQueueResult = device.get_queue(vkb::QueueType::compute);
	DEV_ASSERT(computeQueueResult.has_value(), "RenderManager", "Failed to get Compute queue result!");
	computeQueue = computeQueueResult.value();

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

	// Create DepthBuffer
	VkExtent3D depthImageExtent = {
		swapchain.extent.width,
		swapchain.extent.height,
		1
	};

	depthFormat = VK_FORMAT_D32_SFLOAT;

	VkImageCreateInfo depthImageCreateInfo = {};
	depthImageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	depthImageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
	depthImageCreateInfo.format = depthFormat;
	depthImageCreateInfo.extent = depthImageExtent;
	depthImageCreateInfo.mipLevels = 1;
	depthImageCreateInfo.arrayLayers = 1;
	depthImageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	depthImageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	depthImageCreateInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

	VmaAllocationCreateInfo depthAllocationCreateInfo = {};
	depthAllocationCreateInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
	depthAllocationCreateInfo.requiredFlags = VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	DEV_ASSERT(vmaCreateImage(allocator, &depthImageCreateInfo, &depthAllocationCreateInfo, &depthImage, &depthImageAllocation, nullptr) == VK_SUCCESS, "RenderManager", "Error creating Depth image!");

	VkImageViewCreateInfo depthImageViewCreateInfo = {};
	depthImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	depthImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	depthImageViewCreateInfo.image = depthImage;
	depthImageViewCreateInfo.format = depthFormat;
	depthImageViewCreateInfo.subresourceRange.baseMipLevel = 0;
	depthImageViewCreateInfo.subresourceRange.levelCount = 1;
	depthImageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
	depthImageViewCreateInfo.subresourceRange.layerCount = 1;
	depthImageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

	DEV_ASSERT(vkCreateImageView(device.device, &depthImageViewCreateInfo, nullptr, &depthImageView) == VK_SUCCESS, "RenderManager", "Error creating depth image view!");

	// Create CommandPool (Graphics + Compute)
	graphicsCommandPool.Initialization(device, vkb::QueueType::graphics);
	computeCommandPool.Initialization(device, vkb::QueueType::compute);

	// Create CommandBuffer
	commandBuffer.Initialization(device.device, graphicsCommandPool.GetCommandPool());
	
	// Create vertex Buffers
	skyboxVertexBuffer.Initialization(allocator, 1024);

	// Create createMatrixUBO
	// Create createSSBOs
	// TODO Descriptors
		// TODO Descriptor pool
	std::vector<VkDescriptorPoolSize> poolSizes =
	{
	  { VK_DESCRIPTOR_TYPE_SAMPLER, 10000 },
	  { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 10000 },
	  { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 10000 },
	  { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
	  { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
	  { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
	};

	VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
	descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptorPoolCreateInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	descriptorPoolCreateInfo.maxSets = 10000;
	descriptorPoolCreateInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	descriptorPoolCreateInfo.pPoolSizes = poolSizes.data();

	DEV_ASSERT(vkCreateDescriptorPool(device.device, &descriptorPoolCreateInfo, nullptr, &descriptorPool) == VK_SUCCESS, "RenderManager", "Error creating the Descriptor Pool!");
	DEV_LOG(TE_INFO, "RenderManager", "DescriptorPool created!");

		// TODO Descriptor layouts
	// Skybox
	VkDescriptorSetLayoutBinding skyBoxLayoutBinding = {};
	skyBoxLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	skyBoxLayoutBinding.binding = 0;
	skyBoxLayoutBinding.descriptorCount = 1;
	skyBoxLayoutBinding.pImmutableSamplers = nullptr;
	skyBoxLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutCreateInfo skyBoxLayoutCreateInfo = {};
	skyBoxLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	skyBoxLayoutCreateInfo.bindingCount = 1;
	skyBoxLayoutCreateInfo.pBindings = &skyBoxLayoutBinding;

	DEV_ASSERT(vkCreateDescriptorSetLayout(device.device, &skyBoxLayoutCreateInfo, nullptr, &skyBoxDescriptorSetLayout) == VK_SUCCESS, "RenderManager", "Error creating the descriptor set for the sky box!");
	DEV_LOG(TE_INFO, "RenderManager", "Descriptor Layout created!");

		// TODO Descriptor sets
	VkDescriptorSetAllocateInfo skyBoxAllocate = {};
	skyBoxAllocate.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	skyBoxAllocate.descriptorPool = descriptorPool;
	skyBoxAllocate.descriptorSetCount = 1;
	skyBoxAllocate.pSetLayouts = &skyBoxDescriptorSetLayout;

	DEV_ASSERT(vkAllocateDescriptorSets(device.device, &skyBoxAllocate, &skyBoxDescriptorSet) == VK_SUCCESS, "RenderManager", "Error allocating sky box description set!");
	DEV_LOG(TE_INFO, "RenderManager", "Descriptor Set created!");

	// TODO RenderPass
	renderPass.Initialization(device.device, swapchain.image_format, depthFormat);

		// TODO Pipeline layout
	// Skybox
	VkDescriptorSetLayout rdAssimpTextureDescriptorLayout = VK_NULL_HANDLE;
	VkDescriptorSetLayout rdSkyboxDescriptorLayout = VK_NULL_HANDLE;
	std::vector<VkDescriptorSetLayout> skyboxLayouts =
	{
		rdAssimpTextureDescriptorLayout,
		rdSkyboxDescriptorLayout
	};
	skyboxLayout.Initialization(device, skyboxLayouts);
	// TODO Pipeline
	// Skybox
	std::string vertexShaderFileName = "Shaders/skybox.vert.spv";
	std::string fragmentShaderFileName = "Shader/skybox.frag.spv";
	//skyboxPipeline.Initialization(device, skyboxLayout.GetPipelineLayout(), renderPass.GetRenderPass(), vertexShaderFileName, fragmentShaderFileName);
	
	// Create Pipeline NOTE Maybe need multiple pipelines to avoid creating then in real time
	// Create Framebuffer
	//framebuffer.Initialization(device, swapchain);

	// Create semaphores to sync GPU and CPU (fences)
	fences.Initialization(device.device);
	semaphores.Initialization(device.device);

	// TODO Create matrix to apply translations and rotations


}

void RenderManager::Cleanup()
{	
	DEV_ASSERT(vkDeviceWaitIdle(device.device) == VK_SUCCESS, "RenderManager", "Error waiting the device to be idling!");
	fences.Cleanup(device.device);
	semaphores.Cleanup(device.device);
	framebuffer.Cleanup(device.device);
	commandBuffer.Cleanup(device.device);
	graphicsCommandPool.Cleanup(device.device);
	computeCommandPool.Cleanup(device.device);
	skyboxVertexBuffer.Cleanup(allocator);	
	renderPass.Cleanup(device.device);
	vkFreeDescriptorSets(device.device, descriptorPool, 1, &skyBoxDescriptorSet);
	DEV_LOG(TE_INFO, "RenderManager", "Descriptor Sets destroyed!");
	vkDestroyDescriptorSetLayout(device.device, skyBoxDescriptorSetLayout, nullptr);
	DEV_LOG(TE_INFO, "RenderManager", "Descriptor Set Layout destroyed!");
	vkDestroyDescriptorPool(device.device, descriptorPool, nullptr);
	DEV_LOG(TE_INFO, "RenderManager", "Descriptor Pool destroyed!");
	vkDestroyImageView(device.device, depthImageView, nullptr);
	DEV_LOG(TE_INFO, "RenderManager", "Descriptor Image View destroyed!");
	vmaDestroyImage(allocator, depthImage, depthImageAllocation);
	DEV_LOG(TE_INFO, "RenderManager", "Image destroyed!");
	vmaDestroyAllocator(allocator);
	DEV_LOG(TE_INFO, "RenderManager", "VMA Allocator destroyed!");	
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