#pragma once

#include "../../Logger/Logger.h"
#include <vulkan/vulkan.h>
#include <vma/vk_mem_alloc.h>
#include <assimp/texture.h>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "../stbi-src/stb_image.h"

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
	void LoadCubeTexture(VmaAllocator allocator, VkDevice device, VkPhysicalDevice physicalDevice, TextureData textureData, VkCommandPool commandPool, VkQueue graphicsQeueue, std::string textureFilename, bool flipImage);
	void Cleanup(VmaAllocator allocator, VkDescriptorPool descriptorPool, VkDevice device, TextureData textureData);

private:
	void UploadCubeTextureToGPU(VmaAllocator allocator, VkDevice device, VkPhysicalDevice physicDevice, VkCommandPool commandPool, TextureData textureData, VkTextureStagingBuffer staging, uint32_t width, uint32_t height);
	TextureData textureData;
};