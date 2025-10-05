#include "ComputeShaderPipeline.h"

void ComputeShaderPipeline::Initialization(VkDevice& device, VkPipelineLayout& pipelineLayout, std::string shaderFileName)
{
	Shader shader;
	shader.LoadShader(device, shaderFileName);

	VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo = {};
	pipelineShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	pipelineShaderStageCreateInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
	pipelineShaderStageCreateInfo.module = shader.GetShader();
	pipelineShaderStageCreateInfo.pName = "main";

	VkComputePipelineCreateInfo computePipelineCreateInfo = {};
	computePipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
	computePipelineCreateInfo.layout = pipelineLayout;
	computePipelineCreateInfo.stage = pipelineShaderStageCreateInfo;

	DEV_ASSERT(vkCreateComputePipelines(device, VK_NULL_HANDLE, 1, &computePipelineCreateInfo, nullptr, &pipeline) == VK_SUCCESS, "ComputePipeline", "Error creating the pipeline!");

	shader.Cleanup(device);
}

void ComputeShaderPipeline::Cleanup(VkDevice& device)
{
	vkDestroyPipeline(device, pipeline, nullptr);
	DEV_LOG(TE_INFO, "ComputeShaderPipeline", "Pipeline destroyed!");
}