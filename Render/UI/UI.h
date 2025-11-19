#pragma once

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_vulkan.h>
#include <ImGuiFileDialog.h>
#include <string>
#include <vector>
#include <vulkan/vulkan.hpp>
#include <SDL3/SDL.h>
#include <VkBootstrap.h>
#include "../RenderPass/RenderPass.h"
#include "../CommandBuffer/CommandBuffer.h"
#include "../../Logger/Logger.h"

class UI
{
public:
	void Initialization(VkDevice device, VkPhysicalDevice physicalDevice, vkb::Instance vkbInstance, VkQueue graphicsQueue, vkb::Swapchain swapchain, VkCommandPool commandPool, SDL_Window* window);
	void NewFrame();
	void CreateSettingsWindow();
	void Draw();
	void PrepareCommandBuffer();
	void BeginRenderPass(VkRenderPassBeginInfo renderPassBeginInfo);
	void EndRenderPass();
	VkCommandBuffer GetCommandBuffer() { return commandBuffer.GetCommandBuffer(); }
	void EndCommandBuffer();
	void Cleanup(VkDevice device);
	void Test();

private:
	VkDescriptorPool descriptorPool;
	CommandBuffer commandBuffer;
	RenderPass renderPass;
};

