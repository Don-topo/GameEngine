#pragma once

#include "../../Logger/Logger.h"
#include <vulkan/vulkan.h>
#include <vma/vk_mem_alloc.h>

struct IndexBufferData {
	size_t bufferSize = 0;
	VkBuffer buffer = VK_NULL_HANDLE;
	VmaAllocation bufferAlloc = nullptr;
	VkBuffer stagingBuffer = VK_NULL_HANDLE;
	VmaAllocation stagingBufferAlloc = nullptr;
};

class IndexBuffer
{
public:
	void Initialization(VmaAllocator allocator, size_t sizeToAllocate);
	void Cleanup(VmaAllocator allocator);

private:
	IndexBufferData indexBufferData = {};
};