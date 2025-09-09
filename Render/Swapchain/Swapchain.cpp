#include "Swapchain.h"

void Swapchain::Initialization(VkDevice device)
{
	// TODO Fix this need a vkb device not VkDevice
	/*vkb::SwapchainBuilder swapchainBuilder = {device};
	VkSurfaceFormatKHR surfaceFormatKHR;
	surfaceFormatKHR.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	surfaceFormatKHR.format = VK_FORMAT_B8G8R8A8_UNORM;
	
	vkb::Result <vkb::Swapchain> swapchainResult = swapchainBuilder.
		set_old_swapchain(swapchain).
		set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR).
		set_desired_format(surfaceFormatKHR).
		build();

	DEV_ASSERT(swapchainResult.has_value(), "Swapchain", "Failed to create the SwapChain!");
	swapchain = swapchainResult.value();*/
}

void Swapchain::Cleanup()
{

}