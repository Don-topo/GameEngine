#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "glm/glm.hpp"
#include "vma/vk_mem_alloc.h"
#include "../../Logger/Logger.h"
#include "../IndexBuffer/IndexBuffer.h"
#include "../CommandBuffer/CommandBuffer.h"
#include "../Pipelines/SkyboxPipeline.h"

struct VertexBufferData {
	unsigned int bufferSize = 0;
	void* data = nullptr;
	VkBuffer buffer = VK_NULL_HANDLE;
	VmaAllocation bufferAlloc = VK_NULL_HANDLE;
	VkBuffer stagingBuffer = VK_NULL_HANDLE;
	VmaAllocation stagingBufferAlloc = VK_NULL_HANDLE;
};

struct LineVertex {
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 color = glm::vec3(0.0f);

	LineVertex() {};
	LineVertex(glm::vec3 pos, glm::vec3 col) : position(pos), color(col) {};
};

struct LineMesh {
	std::vector<LineVertex> vertices{};
};

class VertexBuffer
{
public:
	void Initialization(VmaAllocator allocator, unsigned int bufferSize);
	void UploadData(VmaAllocator allocator, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, VkMesh meshData);
	void UploadData(VmaAllocator allocator, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, LineMesh vertexData);
	void UploadData(VmaAllocator allocator, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, SkyboxMesh vertexData);
	void UploadData(VmaAllocator allocator, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, std::vector<glm::vec3> vertexData);
	VertexBufferData GetVertexBuffer() { return vertexBufferData; }
	void Cleanup(VmaAllocator& allocator);

private:
	VertexBufferData vertexBufferData;
	void UploadToGPU(VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue);
};