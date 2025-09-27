#pragma once

#include "../../Logger/Logger.h"
#include <vulkan/vulkan.h>
#include <vma/vk_mem_alloc.h>
#include <assimp/texture.h>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "../stbi-src/stb_image.h"
#include <vector>
#include "../CommandBuffer/CommandBuffer.h"

struct TextureData {
	VkImage image = VK_NULL_HANDLE;
	VkImageView imageView = VK_NULL_HANDLE;
	VkSampler sampler = VK_NULL_HANDLE;
	VmaAllocation imageAlloc = nullptr;

	VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
};

struct VkTextureStagingBuffer {
	VkBuffer stagingBuffer = VK_NULL_HANDLE;
	VmaAllocation stagingBufferAlloc = VK_NULL_HANDLE;
};


class Texture
{
public:
	void LoadTexture(VmaAllocator allocator, VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkDescriptorPool descriptorPool, VkDescriptorSetLayout descriptorSetLayout, VkQueue queue, TextureData texData, std::string textureFilename,
		bool generateMipmaps, bool flipImage);
	void LoadTexture(VmaAllocator allocator, VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkDescriptorPool descriptorPool, VkDescriptorSetLayout descriptorSetLayout, VkQueue queue, TextureData textureData, aiTexel* texelData, std::string fileName, int width, int height, bool generateMipmaps = true, bool flipImage = false);
	void LoadCubeTexture(VmaAllocator allocator, VkDevice device, VkPhysicalDevice physicalDevice, TextureData textureData, VkCommandPool commandPool, VkQueue graphicsQeueue, VkDescriptorPool descriptorPool, VkDescriptorSetLayout descriptorSetLayout, std::string textureFilename, bool flipImage);
	void Cleanup(VmaAllocator allocator, VkDescriptorPool descriptorPool, VkDevice device, TextureData textureData);

private:
	void UploadTextureToGPU(VmaAllocator allocator, VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue queue, TextureData textureData, VkTextureStagingBuffer textureStagingBuffer, VkDescriptorPool descriptorPool, VkDescriptorSetLayout descriptorSetLayout, uint32_t width, uint32_t height, bool generateMipmaps, uint32_t mipmapLevels);
	void UploadCubeTextureToGPU(VmaAllocator allocator, VkDevice device, VkPhysicalDevice physicDevice, VkCommandPool commandPool, VkQueue queue, TextureData textureData, VkTextureStagingBuffer staging, VkDescriptorPool descriptorPool, VkDescriptorSetLayout descriptorSetLayout, uint32_t width, uint32_t height);
	TextureData textureData;
};