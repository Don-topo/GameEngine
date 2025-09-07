#include "RenderManager.h"

void RenderManager::Initialization()
{
	VkInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	vkCreateInstance(&instanceCreateInfo, nullptr, &instance);
}
