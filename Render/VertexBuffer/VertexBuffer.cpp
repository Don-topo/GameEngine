#include "VertexBuffer.h"

void VertexBuffer::Initialization(VmaAllocator allocator, unsigned int bufferSize)
{
	if (bufferSize == 0) bufferSize = 1024;

	VkBufferCreateInfo bufferCreateInfo = {};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.size = bufferSize;
	bufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VmaAllocationCreateInfo bufferAllocationCreateInfo = {};
	bufferAllocationCreateInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

	DEV_ASSERT(vmaCreateBuffer(allocator, &bufferCreateInfo, &bufferAllocationCreateInfo, &vertexBufferData.buffer, &vertexBufferData.bufferAlloc, nullptr), "VertexBuffer", "Error allocating the Vertex Buffer!");

	// Staging
	VkBufferCreateInfo stagingBufferCreateInfo = {};
	stagingBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	stagingBufferCreateInfo.size = bufferSize;
	stagingBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

	VmaAllocationCreateInfo stagingAllocationCreateInfo = {};
	stagingAllocationCreateInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;

	DEV_ASSERT(vmaCreateBuffer(allocator, &stagingBufferCreateInfo, &stagingAllocationCreateInfo, &vertexBufferData.stagingBuffer, &vertexBufferData.stagingBufferAlloc, nullptr), "VertexBuffer", "Error creating Staging Vertex Buffer!");

	vertexBufferData.bufferSize = bufferSize;

}

void VertexBuffer::UploadData(VmaAllocator allocator, VkMesh meshData)
{
	unsigned int vertexDataSize = meshData.vertices.size() * sizeof(VkVertex);

	if (vertexBufferData.bufferSize < vertexDataSize)
	{
		Cleanup(allocator);
		Initialization(allocator, vertexDataSize);
		vertexBufferData.bufferSize = vertexDataSize;
	}

	void* data;

	DEV_ASSERT(vmaMapMemory(allocator, vertexBufferData.stagingBufferAlloc, &data), "VertexBuffer", "Error mapping VertexBuffer!");

}

void VertexBuffer::Cleanup(VmaAllocator allocator)
{
	vmaDestroyBuffer(allocator, vertexBufferData.stagingBuffer, vertexBufferData.stagingBufferAlloc);
	vmaDestroyBuffer(allocator, vertexBufferData.buffer, vertexBufferData.bufferAlloc);
	DEV_LOG(TE_INFO, "VertexBuffer", "Vertex Buffer cleaned!");
}