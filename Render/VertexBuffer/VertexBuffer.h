#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "glm/glm.hpp"
#include "vma/vk_mem_alloc.h"
#include "../../Logger/Logger.h"
#include "../IndexBuffer/IndexBuffer.h"

struct VertexBufferData {
	unsigned int bufferSize = 0;
	void* data = nullptr;
	VkBuffer buffer = VK_NULL_HANDLE;
	VmaAllocation bufferAlloc = VK_NULL_HANDLE;
	VkBuffer stagingBuffer = VK_NULL_HANDLE;
	VmaAllocation stagingBufferAlloc = VK_NULL_HANDLE;
};

class VertexBuffer
{
public:
	void Initialization(VmaAllocator allocator, unsigned int bufferSize);
	void UploadData(VmaAllocator allocator, VkMesh meshData);
	VertexBufferData GetVertexBuffer() { return vertexBufferData; }
	void Cleanup(VmaAllocator allocator);

private:
	VertexBufferData vertexBufferData;
	//void UploadToGPU(VkDevice device, )
};