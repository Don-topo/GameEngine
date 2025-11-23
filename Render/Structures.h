#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <unordered_map>
#include <string>
#include <vulkan/vulkan.h>
#include <assimp/scene.h>
#include <vma/vk_mem_alloc.h>

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

struct IndexBufferData {
	size_t bufferSize = 0;
	VkBuffer buffer = VK_NULL_HANDLE;
	VmaAllocation bufferAlloc = nullptr;
	VkBuffer stagingBuffer = VK_NULL_HANDLE;
	VmaAllocation stagingBufferAlloc = nullptr;
};

enum class cameraType : uint8_t {
	free = 0,
	firstPerson,
	thirdPerson,
	stationary,
	stationaryFollowing
};

enum class cameraProjection : uint8_t {
	perspective = 0,
	orthogonal
};

struct CameraSettings {
	std::string csCamName = "Camera";

	glm::vec3 csWorldPosition = glm::vec3(0.0f);
	float csViewAzimuth = 0.0f;
	float csViewElevation = 0.0f;

	int csFieldOfView = 90;
	float csOrthoScale = 20.0f;

	bool csFirstPersonLockView = true;
	int csFirstPersonBoneToFollow = 0;
	glm::vec3 csFirstPersonOffsets = glm::vec3(0.0f);

	float csThirdPersonDistance = 5.0f;
	float csThirdPersonHeightOffset = 0.0f;

	float csFollowCamHeightOffset = 2.0f;

	cameraType csCamType = cameraType::free;
	cameraProjection csCamProjection = cameraProjection::perspective;
};