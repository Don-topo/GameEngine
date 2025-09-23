#include "Texture.h"

void Texture::LoadCubeTexture(VmaAllocator allocator, VkDevice device, VkPhysicalDevice physicalDevice, TextureData textureData, VkCommandPool commandPool, VkQueue graphicsQeueue, std::string textureFilename, bool flipImage)
{
	int texWidth;
	int texHeight;
	int numberOfChannels;

	stbi_set_flip_vertically_on_load(flipImage);

	unsigned char* textData = stbi_load(textureFilename.c_str(), &texWidth, &texHeight, &numberOfChannels, STBI_rgb_alpha);
	
	if (!textData)
	{
		stbi_image_free(textData);
		return;
	}

	VkDeviceSize deviceSize = texWidth * texHeight * 4;

	VkBufferCreateInfo stagingBufferInfo = {};
	stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	stagingBufferInfo.size = deviceSize;
	stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

	VkBuffer stagingBuffer;
	VmaAllocation stagingAllocation;
	
	VmaAllocationCreateInfo stagingAllocationCreateInfo = {};
	stagingAllocationCreateInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;

	DEV_ASSERT(vmaCreateBuffer(allocator, &stagingBufferInfo, &stagingAllocationCreateInfo, &stagingBuffer, &stagingAllocation, nullptr), "Texture", "Error creating the texture buffer!");

	void* data;
	DEV_ASSERT(vmaMapMemory(allocator, stagingAllocation, &data), "Texture", "Error mapping the texture!");
	std::memcpy(data, textData, deviceSize);
	vmaUnmapMemory(allocator, stagingAllocation);
	vmaFlushAllocation(allocator, stagingAllocation, 0, deviceSize);

	stbi_image_free(textData);

	VkTextureStagingBuffer stagingData = { stagingBuffer, stagingAllocation };

	UploadCubeTextureToGPU(allocator, device, physicalDevice, commandPool, textureData, stagingData, texWidth, texHeight);
}

void Texture::UploadCubeTextureToGPU(VmaAllocator allocator, VkDevice device, VkPhysicalDevice physicDevice, VkCommandPool commandPool, TextureData textureData, VkTextureStagingBuffer staging, uint32_t width, uint32_t height)
{
	VkImageCreateInfo imageCreateInfo = {};
	imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
	imageCreateInfo.extent.height = height;
	imageCreateInfo.extent.width = width;
	imageCreateInfo.extent.depth = 1;
	imageCreateInfo.mipLevels = 1;
	imageCreateInfo.arrayLayers = 1;
	imageCreateInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
	imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageCreateInfo.arrayLayers = 6;
	imageCreateInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;

	VmaAllocationCreateInfo allocationCreateInfo = {};
	allocationCreateInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

	DEV_ASSERT(vmaCreateImage(allocator, &imageCreateInfo, &allocationCreateInfo, &textureData.image, &textureData.imageAlloc, nullptr), "Texture", "Error creating the image of the texture!");


}

void Texture::Cleanup(VmaAllocator allocator, VkDescriptorPool descriptorPool, VkDevice device, TextureData textureData)
{

	vkFreeDescriptorSets(device, descriptorPool, 1, &textureData.descriptorSet);
	vkDestroySampler(device, textureData.sampler, nullptr);
	vkDestroyImageView(device, textureData.imageView, nullptr);
	vmaDestroyImage(allocator, textureData.image, textureData.imageAlloc);
	DEV_LOG(TE_INFO, "Texture", "Texture deleted!");
}