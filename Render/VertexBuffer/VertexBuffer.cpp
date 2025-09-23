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

void VertexBuffer::UploadData(VmaAllocator allocator, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, VkMesh vertexData)
{
	unsigned int dataSize = vertexData.vertices.size() * sizeof(VkVertex);

	if (vertexBufferData.bufferSize < dataSize)
	{
		Cleanup(allocator);
		Initialization(allocator, dataSize);
		vertexBufferData.bufferSize = dataSize;
	}

	void* data;
	DEV_ASSERT(vmaMapMemory(allocator, vertexBufferData.stagingBufferAlloc, &data), "VertexBuffer", "Error allocating memory!");
	std::memcpy(data, vertexData.vertices.data(), dataSize);
	vmaUnmapMemory(allocator, vertexBufferData.stagingBufferAlloc);
	vmaFlushAllocation(allocator, vertexBufferData.stagingBufferAlloc, 0, dataSize);

	UploadToGPU(device, commandPool, graphicsQueue);
}

void VertexBuffer::UploadData(VmaAllocator allocator, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, LineMesh vertexData)
{
	unsigned int vertexDataSize = vertexData.vertices.size() * sizeof(LineMesh);

	if (vertexBufferData.bufferSize < vertexDataSize)
	{
		Cleanup(allocator);
		Initialization(allocator, vertexDataSize);
		vertexBufferData.bufferSize = vertexDataSize;
	}

	void* data;
	DEV_ASSERT(vmaMapMemory(allocator, vertexBufferData.stagingBufferAlloc, &data), "VertexBuffer", "Error mapping the vertex buffer!");
	std::memcpy(data, vertexData.vertices.data(), vertexDataSize);
	vmaUnmapMemory(allocator, vertexBufferData.stagingBufferAlloc);
	vmaFlushAllocation(allocator, vertexBufferData.stagingBufferAlloc, 0, vertexDataSize);

	UploadToGPU(device, commandPool, graphicsQueue);
}

void VertexBuffer::UploadData(VmaAllocator allocator, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, SkyboxMesh vertexData)
{
	unsigned int vertexDataSize = vertexData.vertices.size() * sizeof(SkyboxMesh);

	if (vertexBufferData.bufferSize < vertexDataSize)
	{
		Cleanup(allocator);
		Initialization(allocator, vertexDataSize);
		vertexBufferData.bufferSize = vertexDataSize;
	}

	void* data;
	DEV_ASSERT(vmaMapMemory(allocator, vertexBufferData.stagingBufferAlloc, &data), "VertexBuffer", "Error mapping vertex buffer!");
	std::memcpy(data, vertexData.vertices.data(), vertexDataSize);
	vmaUnmapMemory(allocator, vertexBufferData.stagingBufferAlloc);
	vmaFlushAllocation(allocator, vertexBufferData.stagingBufferAlloc, 0, vertexDataSize);

	UploadToGPU(device, commandPool, graphicsQueue);
}

void VertexBuffer::UploadData(VmaAllocator allocator, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, std::vector<glm::vec3> vertexData)
{
	unsigned int vertexDataSize = vertexData.size() * sizeof(glm::vec3);

	if (vertexBufferData.bufferSize < vertexDataSize)
	{
		Cleanup(allocator);
		Initialization(allocator, vertexDataSize);
		vertexBufferData.bufferSize = vertexDataSize;
	}

	void* data;
	DEV_ASSERT(vmaMapMemory(allocator, vertexBufferData.stagingBufferAlloc, &data), "VertexBuffer", "Error mapping vertex buffer!");
	std::memcpy(data, vertexData.data(), vertexDataSize);
	vmaUnmapMemory(allocator, vertexBufferData.stagingBufferAlloc);
	vmaFlushAllocation(allocator, vertexBufferData.stagingBufferAlloc, 0, vertexDataSize);

	UploadToGPU(device, commandPool, graphicsQueue);
}

void VertexBuffer::UploadToGPU(VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue)
{
	VkBufferMemoryBarrier bufferMemoryBarrier = {};
	bufferMemoryBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
	bufferMemoryBarrier.srcAccessMask = VK_ACCESS_MEMORY_WRITE_BIT;
	bufferMemoryBarrier.dstAccessMask = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
	bufferMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	bufferMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	bufferMemoryBarrier.buffer = vertexBufferData.stagingBuffer;
	bufferMemoryBarrier.offset = 0;
	bufferMemoryBarrier.size = vertexBufferData.bufferSize;

	VkBufferCopy bufferCopy = {};
	bufferCopy.srcOffset = 0;
	bufferCopy.dstOffset = 0;
	bufferCopy.size = vertexBufferData.bufferSize;

	CommandBuffer commandBuffer;
	commandBuffer.CreateSingleShotBuffer(device, commandPool);

	vkCmdCopyBuffer(commandBuffer.GetCommandBuffer(), vertexBufferData.stagingBuffer, vertexBufferData.buffer, 1, &bufferCopy);
	vkCmdPipelineBarrier(commandBuffer.GetCommandBuffer(), VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, 0, 0, nullptr, 1, &bufferMemoryBarrier, 0, nullptr);
	commandBuffer.submitSingleShotBuffer(device, commandPool, graphicsQueue);
	// TODO After this I need to delete this commandBuffer to avoid a memory leak?
}

void VertexBuffer::Cleanup(VmaAllocator allocator)
{
	vmaDestroyBuffer(allocator, vertexBufferData.stagingBuffer, vertexBufferData.stagingBufferAlloc);
	vmaDestroyBuffer(allocator, vertexBufferData.buffer, vertexBufferData.bufferAlloc);
	DEV_LOG(TE_INFO, "VertexBuffer", "Vertex Buffer cleaned!");
}