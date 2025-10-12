#pragma once

#include "../../Logger/Logger.h"
#include <vulkan/vulkan.h>
#include "vma/vk_mem_alloc.h"
#include "glm/glm.hpp"

struct UniformBufferData {
	size_t bufferSize = 0;
	VkBuffer buffer = VK_NULL_HANDLE;
	VmaAllocation bufferAlloc = nullptr;

	VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
};

struct UploadMatrices {
	glm::mat4 viewMatrix{};
	glm::mat4 projectionMatrix{};
	glm::mat4 lightAndFogMatrix{};
};

class UniformBuffer
{
public:
	void Initialization(VmaAllocator& allocator);
	void UploadData(VmaAllocator& allocator, UploadMatrices& matrices);
	UniformBufferData GetUniformBufferData() { return uniformBufferData; }
	void Cleanup(VmaAllocator& allocator);

private:
	UniformBufferData uniformBufferData;
	UploadMatrices uploadMatrices;
};