#include "RenderManager.h"

void RenderManager::Initialization(SDL_Window* window)
{
	this->window = window;

	DEV_ASSERT(SDL_Vulkan_LoadLibrary(NULL), "RenderManager", "Failed to load Vulkan library!");

	InitializeDevice();
	InitializeVMA();
	InitializeQueues();
	CreateSwapchain();
	CreateDepthBuffer();
	CreateCommandPools();
	CreateCommandBuffers();
	CreateVertexBuffers();
	CreateDescriptorPool();
	CreateMatrixUBO();
	CreateSSBOs();
	CreateDescriptorLayouts();
	CreateDescriptorSets();
	CreateRenderPass();
	CreatePipelineLayouts();
	CreatePipelines();
	CreateFramebuffer();
	// Create semaphores to sync GPU and CPU (fences)
	CreateFences();
	CreateSemaphores();
	InitializeUI();

	skyboxModel.Initialization();
	SkyboxMesh skyBoxMesh = skyboxModel.GetSkyboxVertex();
	sphereModel = SphereModel(1.f, 5, 8, glm::vec3(1.f, 1.f, 1.f));
	line = std::make_shared<LineMesh>();
	skyboxVertexBuffer.UploadData(allocator, device.device, graphicsCommandPool.GetCommandPool(), graphicsQueue, skyBoxMesh);
	const std::string skyboxTextureName = "C:\\Users\\ruben\\Desktop\\GameEngine\\GameEngine\\Assets\\Textures\\skybox.jpg";
	skyboxTexture.LoadCubeTexture(allocator, device.device, physicalDevice.physical_device, graphicsCommandPool.GetCommandPool(), graphicsQueue, descriptorPool, rdAssimpTextureDescriptorLayout, skyboxTextureName, false);
}

void RenderManager::InitializeDevice()
{
	// Vulkan instance with vk-bootstrap
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
	vkb::DeviceBuilder deviceBuilder{ physicalDevice };
	vkb::Result<vkb::Device> deviceBuilderReturn = deviceBuilder.build();
	DEV_ASSERT(deviceBuilderReturn.has_value(), "RenderManager", "Error creating the logic device!");
	device = deviceBuilderReturn.value();
	DEV_LOG(TE_INFO, "RenderManager", "Device created!");
}

void RenderManager::InitializeVMA()
{
	VmaAllocatorCreateInfo allocatorInfo{};
	allocatorInfo.physicalDevice = physicalDevice;
	allocatorInfo.device = device.device;
	allocatorInfo.instance = instance.instance;

	DEV_ASSERT(vmaCreateAllocator(&allocatorInfo, &allocator) == VK_SUCCESS, "RenderManager", "Failed to initialize VMA!");
}

void RenderManager::InitializeQueues()
{
	// VK_QUEUES (Present + Compute)
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
}

void RenderManager::CreateSwapchain()
{
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
}

void RenderManager::CreateDepthBuffer()
{
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
}

void RenderManager::CreateCommandPools()
{
	// CommandPool (Graphics + Compute)
	graphicsCommandPool.Initialization(device, vkb::QueueType::graphics);
	computeCommandPool.Initialization(device, vkb::QueueType::compute);
}

void RenderManager::CreateCommandBuffers()
{
	commandBuffer.Initialization(device.device, graphicsCommandPool.GetCommandPool());
	lineCommandBuffer.Initialization(device.device, graphicsCommandPool.GetCommandPool());
}

void RenderManager::CreateVertexBuffers()
{
	skyboxVertexBuffer.Initialization(allocator, 1024);
}

void RenderManager::CreateMatrixUBO()
{
	perspectiveViewMaxtrixUBO.Initialization(allocator);
}

void RenderManager::CreateSSBOs()
{

}

void RenderManager::CreateDescriptorPool()
{
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
}

void RenderManager::CreateDescriptorLayouts()
{
	VkDescriptorSetLayoutBinding assimpTextureBind{};
	assimpTextureBind.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	assimpTextureBind.binding = 0;
	assimpTextureBind.descriptorCount = 1;
	assimpTextureBind.pImmutableSamplers = nullptr;
	assimpTextureBind.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	std::vector<VkDescriptorSetLayoutBinding> assimpTexBindings = { assimpTextureBind };

	VkDescriptorSetLayoutCreateInfo assimpTextureCreateInfo{};
	assimpTextureCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	assimpTextureCreateInfo.bindingCount = static_cast<uint32_t>(assimpTexBindings.size());
	assimpTextureCreateInfo.pBindings = assimpTexBindings.data();

	DEV_ASSERT(vkCreateDescriptorSetLayout(device.device, &assimpTextureCreateInfo, nullptr, &rdAssimpTextureDescriptorLayout) == VK_SUCCESS, "RenderManager", "Error creating the descriptor set Layout for texture");

	/* skybox shader */
	VkDescriptorSetLayoutBinding assimpUboBind{};
	assimpUboBind.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	assimpUboBind.binding = 0;
	assimpUboBind.descriptorCount = 1;
	assimpUboBind.pImmutableSamplers = nullptr;
	assimpUboBind.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutCreateInfo assimpSkyboxCreateInfo{};
	assimpSkyboxCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	assimpSkyboxCreateInfo.bindingCount = 1;
	assimpSkyboxCreateInfo.pBindings = &assimpUboBind;

	DEV_ASSERT(vkCreateDescriptorSetLayout(device.device, &assimpSkyboxCreateInfo, nullptr, &rdSkyboxDescriptorLayout) == VK_SUCCESS, "RenderManager", "Error creating the descriptor layout to skybox!");

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
}

void RenderManager::CreateDescriptorSets()
{
	VkDescriptorSetAllocateInfo skyBoxAllocate = {};
	skyBoxAllocate.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	skyBoxAllocate.descriptorPool = descriptorPool;
	skyBoxAllocate.descriptorSetCount = 1;
	skyBoxAllocate.pSetLayouts = &rdSkyboxDescriptorLayout;

	DEV_ASSERT(vkAllocateDescriptorSets(device.device, &skyBoxAllocate, &skyBoxDescriptorSet) == VK_SUCCESS, "RenderManager", "Error allocating sky box description set!");
	DEV_LOG(TE_INFO, "RenderManager", "Descriptor Set created!");

	UpdateDescriptorSets();
}

void RenderManager::CreateRenderPass()
{
	renderPass.Initialization(device.device, swapchain.image_format, depthFormat);
}

void RenderManager::CreatePipelineLayouts()
{
	// Skybox
	std::vector<VkDescriptorSetLayout> skyboxLayouts =
	{
		rdAssimpTextureDescriptorLayout,
		rdSkyboxDescriptorLayout
	};
	skyboxLayout.Initialization(device.device, skyboxLayouts);
}

void RenderManager::CreatePipelines()
{
	// Skybox
	std::string vertexShaderFileName = "Shaders/skybox.vert.spv";
	std::string fragmentShaderFileName = "Shaders/skybox.frag.spv";
	skyboxPipeline.Initialization(device.device, skyboxLayout.GetPipelineLayout(), renderPass.GetRenderPass(), vertexShaderFileName, fragmentShaderFileName);
}

void RenderManager::CreateFramebuffer()
{
	framebuffer.Initialization(device.device, renderPass.GetRenderPass(), depthImageView, swapchain);
}

void RenderManager::CreateFences()
{
	fences.Initialization(device.device);
}

void RenderManager::CreateSemaphores()
{	
	semaphores.Initialization(device.device);
}

void RenderManager::InitializeUI()
{
	ui.Initialization(device, physicalDevice, instance, graphicsQueue, swapchain, window);
}

void RenderManager::RecreateSwapchain()
{
	
	SDL_GetWindowSizeInPixels(window, &width, &height);
	if (width == 0 || height == 0)
	{
		SDL_GetWindowSizeInPixels(window, &width, &height);
		SDL_WaitEvent(NULL);
	}
	DEV_ASSERT(vkDeviceWaitIdle(device.device) == VK_SUCCESS, "RenderManager", "Error waiting de cpu!");
	framebuffer.Cleanup(device.device);
	vkDestroyImageView(device.device, depthImageView, nullptr);
	vmaDestroyImage(allocator, depthImage, depthImageAllocation);
	swapchain.destroy_image_views(swapchainImageViews);
	CreateSwapchain();
	CreateDepthBuffer();
	CreateFramebuffer();
}

void RenderManager::UpdateDescriptorSets()
{
	// Skybox shader
	VkDescriptorBufferInfo skyboxMatrixInfo = {};
	skyboxMatrixInfo.buffer = perspectiveViewMaxtrixUBO.GetUniformBufferData().buffer;
	skyboxMatrixInfo.offset = 0;
	skyboxMatrixInfo.range = VK_WHOLE_SIZE;

	VkWriteDescriptorSet skyboxMatrixWriteDescriptorSet = {};
	skyboxMatrixWriteDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	skyboxMatrixWriteDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	skyboxMatrixWriteDescriptorSet.dstSet = skyBoxDescriptorSet;
	skyboxMatrixWriteDescriptorSet.dstBinding = 0;
	skyboxMatrixWriteDescriptorSet.descriptorCount = 1;
	skyboxMatrixWriteDescriptorSet.pBufferInfo = &skyboxMatrixInfo;

	std::vector<VkWriteDescriptorSet> writeDescriptorSets = { skyboxMatrixWriteDescriptorSet };

	vkUpdateDescriptorSets(device.device, static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);
}

void RenderManager::Update()
{
	// Wait for fences
	std::vector<VkFence> fenc = { fences.GetRenderFence(), fences.GetComputeFence() };
	DEV_ASSERT(vkWaitForFences(device.device, static_cast<uint32_t>(fenc.size()), fenc.data(), VK_TRUE, UINT64_MAX) == VK_SUCCESS, "RenderManager", "Failed to wait the fences!");

	// Get the next Image
	uint32_t currentIndexImage = 0;
	VkResult result = vkAcquireNextImageKHR(device.device, swapchain.swapchain, UINT64_MAX, semaphores.GetPresentSemaphore(), VK_NULL_HANDLE, &currentIndexImage);

	// Check if we need to recreate the swapchain
	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		// RecreateSwapchain
		RecreateSwapchain();
	}
	// The only valid results are succes and suboptimal khr
	DEV_ASSERT(result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR, "RenderManager", "Error obtaining the next swapchain image!");

	// do an empty submit if we don't have animated models to satisfy fence and semaphore  (to avoid blockings)
	VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;

	VkSubmitInfo computeSubmitInfo{};
	computeSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	computeSubmitInfo.waitSemaphoreCount = 1;
	computeSubmitInfo.pWaitSemaphores = &semaphores.GetGraphicsSemaphore();
	computeSubmitInfo.pWaitDstStageMask = &waitStage;

	DEV_ASSERT(vkResetFences(device.device, 1, &fences.GetComputeFence()) == VK_SUCCESS, "RenderManager", "Error reseting the compute fences");
	DEV_ASSERT(vkQueueSubmit(computeQueue, 1, &computeSubmitInfo, fences.GetComputeFence()) == VK_SUCCESS, "RenderManager", "Error submiting compute buffer");

	/* we must wait for the compute shaders to finish before we can read the bone data */
	DEV_ASSERT(vkWaitForFences(device.device, 1, &fences.GetComputeFence(), VK_TRUE, UINT64_MAX) == VK_SUCCESS, "RenderManager", "Error waiting the compute fence!");

	// Reset fences before recording commands	
	DEV_ASSERT(vkResetFences(device.device, 1, &fences.GetComputeFence()) == VK_SUCCESS, "RenderManager", "Error reseting the compute fences");

	// Render Graphics
	DEV_ASSERT(vkResetFences(device.device, 1, &fences.GetRenderFence()) == VK_SUCCESS, "RenderManager", "Error reseting the render fences!");
	commandBuffer.Reset(0);
	commandBuffer.BeginSingleShot();

	std::vector<VkClearValue> colorClearValues = {};
	VkClearValue colorClearValue;
	colorClearValue.color = { { 0.25f, 0.25f, 0.25f, 1.0f } };
	colorClearValues.push_back(colorClearValue);

	VkClearValue depthValue;
	depthValue.depthStencil.depth = 1.0f;

	std::vector<VkClearValue> clearValues = {};
	clearValues.insert(clearValues.end(), colorClearValues.begin(), colorClearValues.end());
	clearValues.push_back(depthValue);

	VkRenderPassBeginInfo renderPassBeginInfo = {};
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.renderPass = renderPass.GetRenderPass();
	renderPassBeginInfo.framebuffer = framebuffer.GetFrameBuffers().at(currentIndexImage);
	renderPassBeginInfo.renderArea.offset = { 0, 0 };
	renderPassBeginInfo.renderArea.extent = swapchain.extent;
	renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassBeginInfo.pClearValues = clearValues.data();

	VkViewport viewport = {};
	viewport.x = 0.f;
	viewport.y = static_cast<float>(swapchain.extent.height);
	viewport.width = static_cast<float>(swapchain.extent.width);
	viewport.height = static_cast<float>(swapchain.extent.height);
	viewport.minDepth = 0.f;
	viewport.maxDepth = 1.f;

	VkRect2D scissor = {};
	scissor.offset = { 0,0 };
	scissor.extent = swapchain.extent;

	vkCmdSetViewport(commandBuffer.GetCommandBuffer(), 0, 1, &viewport);
	vkCmdSetScissor(commandBuffer.GetCommandBuffer(), 0, 1, &scissor);

	vkCmdBeginRenderPass(commandBuffer.GetCommandBuffer(), &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

	// Draw skybox
	vkCmdBindPipeline(commandBuffer.GetCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, skyboxPipeline.GetSkyboxPipeline());
	// Load skybox texture on initialization
	vkCmdBindDescriptorSets(commandBuffer.GetCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, skyboxLayout.GetPipelineLayout(), 0, 1, &skyboxTexture.GetTextureData().descriptorSet, 0, nullptr);
	vkCmdBindDescriptorSets(commandBuffer.GetCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, skyboxLayout.GetPipelineLayout(), 1, 1, &skyBoxDescriptorSet, 0, nullptr);

	VkDeviceSize sizeOffset = 0;
	vkCmdBindVertexBuffers(commandBuffer.GetCommandBuffer(), 0, 1, &skyboxVertexBuffer.GetVertexBuffer().buffer, &sizeOffset);
	// Put the skytexture inside sphere model => Implement sphere + load
	vkCmdDraw(commandBuffer.GetCommandBuffer(), static_cast<uint32_t>(sphereModel.GetSphereVertexs().vertices.size()), 1, 0, 0);
	vkCmdEndRenderPass(commandBuffer.GetCommandBuffer());

	commandBuffer.End();

	std::vector<VkSemaphore> waitSemaphores = { semaphores.GetPresentSemaphore() };
	std::vector<VkSemaphore> signalSemaphores = { semaphores.GetRenderSemaphore(), semaphores.GetGraphicsSemaphore() };
	std::vector<VkPipelineStageFlags> waitStageFlags = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	std::vector<VkCommandBuffer> commandBuffers = { commandBuffer.GetCommandBuffer() };
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size());
	submitInfo.pWaitSemaphores = waitSemaphores.data();
	submitInfo.signalSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size());
	submitInfo.pSignalSemaphores = signalSemaphores.data();
	submitInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
	submitInfo.pCommandBuffers = commandBuffers.data();
	submitInfo.pWaitDstStageMask = waitStageFlags.data();

	DEV_ASSERT(vkQueueSubmit(graphicsQueue, 1, &submitInfo, fences.GetRenderFence()) == VK_SUCCESS, "RenderManager", "Error submiting the draw command buffer!");
	
	vkQueueWaitIdle(graphicsQueue);

	VkPresentInfoKHR presentInfoKHR = {};
	presentInfoKHR.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfoKHR.waitSemaphoreCount = 1;
	presentInfoKHR.pWaitSemaphores = &semaphores.GetRenderSemaphore();
	presentInfoKHR.swapchainCount = 1;
	presentInfoKHR.pSwapchains = &swapchain.swapchain;
	presentInfoKHR.pImageIndices = &currentIndexImage;

	result = vkQueuePresentKHR(presentQueue, &presentInfoKHR);
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
	{
		RecreateSwapchain();
	}
	else if (result != VK_SUCCESS)
	{
		DEV_LOG(TE_ERROR, "RenderManager", "Error presenting the next image!");
	}
}

void RenderManager::Cleanup()
{	
	DEV_ASSERT(vkDeviceWaitIdle(device.device) == VK_SUCCESS, "RenderManager", "Error waiting the device to be idling!");

	ui.Cleanup(device.device);
	fences.Cleanup(device.device);
	semaphores.Cleanup(device.device);
	commandBuffer.Cleanup(device.device);	
	lineCommandBuffer.Cleanup(device.device);
	graphicsCommandPool.Cleanup(device.device);
	computeCommandPool.Cleanup(device.device);
	skyboxVertexBuffer.Cleanup(allocator);	
	framebuffer.Cleanup(device.device);
	skyboxPipeline.Cleanup(device.device);
	skyboxLayout.Cleanup(device.device);
	renderPass.Cleanup(device.device);
	skyboxTexture.Cleanup(allocator, descriptorPool, device.device);
	vkFreeDescriptorSets(device.device, descriptorPool, 1, &skyBoxDescriptorSet);
	DEV_LOG(TE_INFO, "RenderManager", "Descriptor Sets destroyed!");
	perspectiveViewMaxtrixUBO.Cleanup(allocator);
	vkDestroyDescriptorSetLayout(device.device, skyBoxDescriptorSetLayout, nullptr);
	vkDestroyDescriptorSetLayout(device.device, rdAssimpTextureDescriptorLayout, nullptr);
	vkDestroyDescriptorSetLayout(device.device, rdSkyboxDescriptorLayout, nullptr);

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