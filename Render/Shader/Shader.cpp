#include "Shader.h"

#include "../../File/FileManager.h"

void Shader::LoadShader(VkDevice device, std::string fileName)
{
	std::string shaderContent = FileManager::LoadFile(fileName);

	VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
	shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderModuleCreateInfo.codeSize = shaderContent.size();
	shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(shaderContent.c_str());

	DEV_ASSERT(vkCreateShaderModule(device, &shaderModuleCreateInfo, nullptr, &shaderModule), "Shader", "Error creating the shader module!");
}

void Shader::Cleanup(VkDevice device)
{
	vkDestroyShaderModule(device, shaderModule, nullptr);
	DEV_LOG(TE_INFO, "Shader", "Shader module cleaned!");
}