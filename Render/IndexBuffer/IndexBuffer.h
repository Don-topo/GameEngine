#pragma once

#include "../../Logger/Logger.h"
#include <vulkan/vulkan.h>
#include <vma/vk_mem_alloc.h>
#include "glm/glm.hpp"
#include <unordered_map>
#include <vector>
#include <assimp/material.h>
#include "../CommandBuffer/CommandBuffer.h"
#include "../../Structures.h"

class IndexBuffer
{
public:
	void Initialization(VmaAllocator& allocator, size_t sizeToAllocate);
	void UploadData(VmaAllocator& allocator, VkDevice& device, VkCommandPool& commandPool, VkQueue& queue, VkMesh& vertexData);
	void Cleanup(VmaAllocator& allocator, VkDevice& device);

private:
	IndexBufferData indexBufferData = {};
	CommandBuffer commandBuffer;
};