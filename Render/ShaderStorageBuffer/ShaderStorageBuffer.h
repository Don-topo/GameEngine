#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <glm/glm.hpp>
#include "../../AABB/AABB.h"
#include "vk_mem_alloc.h"
#include "../../Logger/Logger.h"

struct VkShaderStorageBufferData {
	size_t bufferSize = 0;
	VkBuffer buffer = VK_NULL_HANDLE;
	VmaAllocation bufferAlloc = nullptr;

	VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
};

class ShaderStorageBuffer
{
public:
	void Initialize();
	void GetShaderStorageBuffer();
	void Cleanup(VmaAllocator allocator, VkQueue queue);
private:
	
};

