#pragma once

#include "../../Logger/Logger.h"
#include <vulkan/vulkan.h>
#include <vma/vk_mem_alloc.h>
#include "glm/glm.hpp"
#include <unordered_map>
#include <vector>
#include <assimp/material.h>
#include "../CommandBuffer/CommandBuffer.h"

struct IndexBufferData {
	size_t bufferSize = 0;
	VkBuffer buffer = VK_NULL_HANDLE;
	VmaAllocation bufferAlloc = nullptr;
	VkBuffer stagingBuffer = VK_NULL_HANDLE;
	VmaAllocation stagingBufferAlloc = nullptr;
};

// TODO Move the next structures to another class
struct VkMorphVertex {
	glm::vec4 position = glm::vec4(0.0f);
	glm::vec4 normal = glm::vec4(0.0f);
};

struct VkMorphMesh {
	std::vector<VkMorphVertex> morphVertices{};
};

struct VkVertex {
	glm::vec4 position = glm::vec4(0.0f); // last float is uv.x
	glm::vec4 color = glm::vec4(1.0f);
	glm::vec4 normal = glm::vec4(0.0f); // last float is uv.y
	glm::uvec4 boneNumber = glm::uvec4(0);
	glm::vec4 boneWeight = glm::vec4(0.0f);
};

struct VkMesh {
	std::vector<VkVertex> vertices{};
	std::vector<uint32_t> indices{};
	std::unordered_map<aiTextureType, std::string> textures{};
	bool usesPBRColors = false;
	/* store optional morph meshes directly in renderer mesh */
	std::vector<VkMorphMesh> morphMeshes{};
};


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