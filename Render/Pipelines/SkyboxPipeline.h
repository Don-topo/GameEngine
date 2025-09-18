#pragma once

#include "../../Logger/Logger.h"
#include "../Shader/Shader.h"
#include <vulkan/vulkan.h>
#include <string>
#include <memory>

class SkyboxPipeline
{
public:
	void Initialization(VkDevice device, VkPipelineLayout pipelineLayout, VkRenderPass renderpass, std::string vertexShaderFilename, std::string fragmentShaderFilename);
	void Cleanup(VkDevice device);

private:
	VkPipeline pipeline;
};