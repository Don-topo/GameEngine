#pragma once

#include <vulkan/vulkan.h>
#include <string>
#include "../../Logger/Logger.h"
#include "../Shader/Shader.h"

class ComputeShaderPipeline
{
public:
	void Initialization(VkDevice& device, VkPipelineLayout& pipelineLayout, std::string shaderFileName);
	VkPipeline& GetComputeShaderPipeline() { return pipeline; }
	void Cleanup(VkDevice& device);
private:
	VkPipeline pipeline;
};