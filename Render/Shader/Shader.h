#pragma once

#include "vulkan/vulkan.h"
#include "../../Logger/Logger.h"
#include <string>

class Shader
{
public:
	Shader();
	~Shader();
	void LoadShader(VkDevice device, std::string fileName);
	VkShaderModule GetShader() { return shaderModule; }
	void Cleanup(VkDevice device);

private:
	VkShaderModule shaderModule;
};