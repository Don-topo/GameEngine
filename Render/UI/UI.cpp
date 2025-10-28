#include "UI.h"

void UI::Initialization(VkDevice device, VkPhysicalDevice physicalDevice, vkb::Instance vkbInstance, VkQueue graphicsQueue, vkb::Swapchain swapchain, SDL_Window* window)
{
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();

	std::vector<VkDescriptorPoolSize> descriptorPoolSizes =
	{
	  { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
	  { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
	  { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
	  { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
	  { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
	  { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
	  { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
	  { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
	  { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
	  { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
	  { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
	};

	VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
	descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptorPoolCreateInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	descriptorPoolCreateInfo.maxSets = 1000;
	descriptorPoolCreateInfo.poolSizeCount = static_cast<uint32_t>(descriptorPoolSizes.size());
	descriptorPoolCreateInfo.pPoolSizes = descriptorPoolSizes.data();

	DEV_ASSERT(vkCreateDescriptorPool(device, &descriptorPoolCreateInfo, nullptr, &descriptorPool) == VK_SUCCESS, "UI", "Error creating the descriptor pool!");
	
	DEV_ASSERT(ImGui_ImplSDL3_InitForVulkan(window), "UI", "Error creating ImGui window!");
	DEV_LOG(TE_INFO, "UI", "SLD3 ImGui initializated!");

	VkFormat format;
	format = VK_FORMAT_D32_SFLOAT;
	renderPass.Initialization(device, swapchain.image_format, format);

	ImGui_ImplVulkan_InitInfo vulkanInfo = {};
	vulkanInfo.Instance = vkbInstance.instance;
	vulkanInfo.PhysicalDevice = physicalDevice;
	vulkanInfo.Device = device;
	vulkanInfo.Queue = graphicsQueue;
	vulkanInfo.DescriptorPool = descriptorPool;
	vulkanInfo.MinImageCount = 2;
	vulkanInfo.ImageCount = static_cast<uint32_t>(swapchain.get_images().value().size());
	vulkanInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
	vulkanInfo.RenderPass = renderPass.GetRenderPass();

	DEV_ASSERT(ImGui_ImplVulkan_Init(&vulkanInfo), "UI", "Error initializating ImGui vulkan!");
	DEV_LOG(TE_INFO, "UI", "Vulkan ImGui Initializated!");

	ImGui::StyleColorsDark();
	DEV_LOG(TE_INFO, "UI", "UI Initializated!");
}

void UI::NewFrame()
{
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplSDL3_NewFrame();
}

void UI::Draw()
{
	ImGui::Render();
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer.GetCommandBuffer());
}

void UI::Cleanup(VkDevice device)
{
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	vkDestroyDescriptorPool(device, descriptorPool, nullptr);
	ImGui::DestroyContext();
	DEV_LOG(TE_INFO, "UI", "ImGui released!");
}