#include "Shader.h"

void Shader::LoadShader(VkDevice device, std::string fileName)
{
	VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
	shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	
	// TODO Need to load the shader info fron file (compiled to spir-v)

	DEV_ASSERT(vkCreateShaderModule(device, &shaderModuleCreateInfo, nullptr, &shaderModule), "Shader", "Error creating the shader module => %s", fileName.c_str());
}

void Shader::Cleanup(VkDevice device)
{
	vkDestroyShaderModule(device, shaderModule, nullptr);
	DEV_LOG(TE_INFO, "Shader", "Shader module cleaned!");
}