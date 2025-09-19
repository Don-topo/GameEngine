#pragma once

#include "../../Logger/Logger.h"
#include "../Shader/Shader.h"
#include <vulkan/vulkan.h>
#include <VkBootstrap.h>
#include <string>
#include <memory>
#include <glm/glm.hpp>

struct SkyboxVertex {
	glm::vec4 position = glm::vec4(0.0f);
};

struct SkyboxMesh {
	std::vector<SkyboxVertex> vertices{};
};

class SkyboxPipeline
{
public:
	void Initialization(VkDevice device, VkPipelineLayout pipelineLayout, VkRenderPass renderpass, std::string vertexShaderFilename, std::string fragmentShaderFilename);
	void Cleanup(VkDevice device);

private:
	VkPipeline pipeline;
};