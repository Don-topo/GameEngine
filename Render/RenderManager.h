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
#include "CommandBuffer/CommandBuffer.h"
#include "PipelineLayout/PipelineLayout.h"
#include "Pipelines/SkyboxPipeline.h"
#include "Framebuffer/Framebuffer.h"
#include "Syncronization/SyncFences.h"
#include "Syncronization/SyncSemaphores.h"
#include "RenderPass/RenderPass.h"
#include "UniformBuffer/UniformBuffer.h"
#include "VertexBuffer/VertexBuffer.h"
#include "../Animation/Models/Skybox/SkyboxModel.h"
#include "../Animation/Models/Sphere/SphereModel.h"
#include "Texture/Texture.h"

class RenderManager
{
public:
	void Initialization(SDL_Window* window);
	void Update();
	void Cleanup();
private:
	void InitializeDevice();
	void InitializeVMA();
	void InitializeQueues();
	void CreateSwapchain();
	void CreateDepthBuffer();
	void CreateCommandPools();
	void CreateCommandBuffers();
	void CreateVertexBuffers();
	void CreateMatrixUBO();
	void CreateSSBOs();
	void CreateDescriptorPool();
	void CreateDescriptorLayouts();
	void CreateDescriptorSets();
	void CreateRenderPass();
	void CreatePipelineLayouts();
	void CreatePipelines();
	void CreateFramebuffer();
	void CreateFences();
	void CreateSemaphores();

	void RecreateSwapchain();

	SDL_Window* window;
	vkb::Instance instance;
	VkSurfaceKHR surface;
	vkb::PhysicalDevice physicalDevice;
	vkb::Device device;
	VmaAllocator allocator;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	VkQueue computeQueue;
	vkb::Swapchain swapchain;
	std::vector<VkImage> swapchainImages;
	std::vector<VkImageView> swapchainImageViews;
	Framebuffer framebuffer;
	CommandPool graphicsCommandPool;
	CommandPool computeCommandPool;
	CommandBuffer commandBuffer;
	PipelineLayout skyboxLayout;
	SkyboxPipeline skyboxPipeline;
	SyncFences fences;
	UniformBuffer perspectiveViewMaxtrixUBO;
	SyncSemaphores semaphores;
	RenderPass renderPass;
	VkFormat depthFormat = VK_FORMAT_UNDEFINED;
	VkImage depthImage = VK_NULL_HANDLE;
	VmaAllocation depthImageAllocation = VK_NULL_HANDLE;
	VkImageView depthImageView = VK_NULL_HANDLE;
	VertexBuffer skyboxVertexBuffer;
	VkDescriptorPool descriptorPool;
	VkDescriptorSetLayout skyBoxDescriptorSetLayout = VK_NULL_HANDLE;
	VkDescriptorSet skyBoxDescriptorSet;
	VkDescriptorSetLayout rdAssimpTextureDescriptorLayout = VK_NULL_HANDLE;
	VkDescriptorSetLayout rdSkyboxDescriptorLayout = VK_NULL_HANDLE;

	SkyboxModel skyboxModel = {};
	SphereModel sphereModel = {};
	Texture skyboxTexture;
};
