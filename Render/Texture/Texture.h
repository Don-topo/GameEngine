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

class Texture
{
public:
	void LoadTexture();
	void LoadCubeTexture();
	void Cleanup(VkDevice device);

private:
	void UploadTextureToGPU();
	void UploadCubeTextureToGPU();
	TextureData textureData;
};