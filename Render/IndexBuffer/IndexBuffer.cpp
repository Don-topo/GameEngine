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

void IndexBuffer::UploadData(VmaAllocator allocator, VkDevice device, VkCommandPool commandPool, VkQueue queue, VkMesh vertexData)
{
	unsigned int vertexIndicesSize = vertexData.indices.size() * sizeof(uint32_t);
	if (indexBufferData.bufferSize < vertexIndicesSize)
	{
		Cleanup(allocator, device);
		Initialization(allocator, vertexIndicesSize);
	}

	void* data;

	DEV_ASSERT(vmaMapMemory(allocator, indexBufferData.stagingBufferAlloc, &data), "IndexBuffer", "Error allocating IndexBuffer!");
	std::memcpy(data, vertexData.indices.data(), vertexIndicesSize);
	vmaUnmapMemory(allocator, indexBufferData.stagingBufferAlloc);
	vmaFlushAllocation(allocator, indexBufferData.stagingBufferAlloc, 0, vertexIndicesSize);

	VkBufferMemoryBarrier bufferMemoryBarrier = {};
	bufferMemoryBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
	bufferMemoryBarrier.srcAccessMask = VK_ACCESS_MEMORY_WRITE_BIT;
	bufferMemoryBarrier.dstAccessMask = VK_ACCESS_2_VERTEX_ATTRIBUTE_READ_BIT;
	bufferMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	bufferMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	bufferMemoryBarrier.buffer = indexBufferData.stagingBuffer;
	bufferMemoryBarrier.offset = 0;
	bufferMemoryBarrier.size = indexBufferData.bufferSize;

	VkBufferCopy bufferCopy = {};
	bufferCopy.srcOffset = 0;
	bufferCopy.dstOffset = 0;
	bufferCopy.size = indexBufferData.bufferSize;	
	
	commandBuffer.CreateSingleShotBuffer(device, commandPool);
	vkCmdCopyBuffer(commandBuffer.GetCommandBuffer(), indexBufferData.stagingBuffer, indexBufferData.buffer, 1, &bufferCopy);
	vkCmdPipelineBarrier(commandBuffer.GetCommandBuffer(), VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, 0, 0, nullptr, 1, &bufferMemoryBarrier, 0, nullptr);

	commandBuffer.submitSingleShotBuffer(device, commandPool, queue);
}

void IndexBuffer::Cleanup(VmaAllocator allocator, VkDevice device)
{	
	vmaDestroyBuffer(allocator, indexBufferData.stagingBuffer, indexBufferData.stagingBufferAlloc);
	vmaDestroyBuffer(allocator, indexBufferData.buffer, indexBufferData.bufferAlloc);

	commandBuffer.Cleanup(device);

	DEV_LOG(TE_INFO, "IndexBuffer", "IndexBuffers destroyed!");
}