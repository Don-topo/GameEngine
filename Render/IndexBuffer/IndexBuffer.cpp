#include "IndexBuffer.h"

void IndexBuffer::Initialization(VmaAllocator allocator, size_t sizeToAllocate)
{
	VkBufferCreateInfo bufferCreateInfo = {};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.size = sizeToAllocate;
	bufferCreateInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VmaAllocationCreateInfo allocationCreateInfo = {};
	bufferCreateInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

	DEV_ASSERT(vmaCreateBuffer(allocator, &bufferCreateInfo, &allocationCreateInfo, &indexBufferData.buffer, &indexBufferData.bufferAlloc, nullptr), "IndexBuffer", "Error creating VMA IndexBuffer!");

	VkBufferCreateInfo stagingBufferInfo{};
	stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	stagingBufferInfo.size = sizeToAllocate;
	stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

	VmaAllocationCreateInfo stagingAllocInfo{};
	stagingAllocInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;

	DEV_ASSERT(vmaCreateBuffer(allocator, &stagingBufferInfo, &stagingAllocInfo, &indexBufferData.stagingBuffer, &indexBufferData.stagingBufferAlloc, nullptr), "IndexBuffer", "Error creating VMA StagingBuffer!");

	indexBufferData.bufferSize = sizeToAllocate;
}

void IndexBuffer::Cleanup(VmaAllocator allocator)
{
	vmaDestroyBuffer(allocator, indexBufferData.stagingBuffer, indexBufferData.stagingBufferAlloc);
	vmaDestroyBuffer(allocator, indexBufferData.buffer, indexBufferData.bufferAlloc);
	DEV_LOG(TE_INFO, "IndexBuffer", "IndexBuffers destroyed!");
}