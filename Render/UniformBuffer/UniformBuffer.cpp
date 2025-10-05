#include "UniformBuffer.h"

void UniformBuffer::Initialization(VmaAllocator& allocator)
{
	VkBufferCreateInfo bufferCreateInfo = {};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.size = sizeof(UploadMatrices);
	bufferCreateInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

	VmaAllocationCreateInfo vmaAllocationCreateInfo = {};
	vmaAllocationCreateInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

	DEV_ASSERT(vmaCreateBuffer(allocator, &bufferCreateInfo, &vmaAllocationCreateInfo, &uniformBufferData.buffer, &uniformBufferData.bufferAlloc, nullptr) == VK_SUCCESS, "UniformBuffer", "Error creating the Uniform Buffer!");
}

void UniformBuffer::UploadData(VmaAllocator& allocator, UploadMatrices& matrices)
{
	void* data = nullptr;
	DEV_ASSERT(vmaMapMemory(allocator, uniformBufferData.bufferAlloc, &data) == VK_SUCCESS, "UniformBuffer", "Error mapping the Uniform Mapping!");
	std::memcpy(data, &matrices, sizeof(UploadMatrices));
	vmaUnmapMemory(allocator, uniformBufferData.bufferAlloc);
	vmaFlushAllocation(allocator, uniformBufferData.bufferAlloc, 0, uniformBufferData.bufferSize);
}

void UniformBuffer::Cleanup(VmaAllocator& allocator)
{
	vmaDestroyBuffer(allocator, uniformBufferData.buffer, uniformBufferData.bufferAlloc);
	DEV_LOG(TE_INFO, "UniformBuffer", "Uniform Buffer deleted!");
}