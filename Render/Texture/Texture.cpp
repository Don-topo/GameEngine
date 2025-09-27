#include "Texture.h"

void Texture::LoadTexture(VmaAllocator allocator, VkDevice device, TextureData texData, std::string textureFilename,
	bool generateMipmaps, bool flipImage)
{
	int texWidth;
	int texHeight;
	int numberOfChannels;
	uint32_t mipmapLevels = 1;

	stbi_set_flip_vertically_on_load(flipImage);

	unsigned char* data = stbi_load(textureFilename.c_str(), &texWidth, &texHeight, &numberOfChannels, STBI_rgb_alpha);

	if (data != nullptr)
	{
		stbi_image_free(data);
		DEV_ASSERT(data != nullptr, "Texture", "Error loading the texture file %s", textureFilename.c_str());
	}

	if (generateMipmaps)
	{
		mipmapLevels += static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight))));
	}

	VkDeviceSize textureSize = texWidth * texHeight * 4;

	VkBufferCreateInfo stagingBufferCreateInfo = {};
	stagingBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	stagingBufferCreateInfo.size = textureSize;
	stagingBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

	VkBuffer stagingBuffer;
	VmaAllocation stagingAllocator;

	VmaAllocationCreateInfo vmaStagingAllocatorCreateInfo = {};
	vmaStagingAllocatorCreateInfo.usage = VMA_MEMORY_USAGE_CPU_COPY;

	DEV_ASSERT(vmaCreateBuffer(allocator, &stagingBufferCreateInfo, &vmaStagingAllocatorCreateInfo, &stagingBuffer, &stagingAllocator, nullptr), "Texture", "Error allocating the texture buffer!");

	void* dataToUpload;

	DEV_ASSERT(vmaMapMemory(allocator, stagingAllocator, &dataToUpload), "Texture", "Error mapping the texture!");

	std::memcpy()
}

void Texture::LoadCubeTexture(VmaAllocator allocator, VkDevice device, VkPhysicalDevice physicalDevice, TextureData textureData, VkCommandPool commandPool, VkQueue graphicsQeueue, VkDescriptorPool descriptorPool, VkDescriptorSetLayout descriptorSetLayout, std::string textureFilename, bool flipImage)
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

	UploadCubeTextureToGPU(allocator, device, physicalDevice, commandPool, graphicsQeueue, textureData, stagingData, descriptorPool, descriptorSetLayout, texWidth, texHeight);
}

void Texture::UploadTextureToGPU(VmaAllocator allocator, VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue queue, TextureData textureData, VkTextureStagingBuffer textureStagingBuffer, VkDescriptorPool descriptorPool, VkDescriptorSetLayout descriptorSetLayout, uint32_t width, uint32_t height, bool generateMipmaps)
{

}

void Texture::UploadCubeTextureToGPU(VmaAllocator allocator, VkDevice device, VkPhysicalDevice physicDevice, VkCommandPool commandPool, VkQueue queue, TextureData textureData, VkTextureStagingBuffer staging, VkDescriptorPool descriptorPool, VkDescriptorSetLayout descriptorSetLayout, uint32_t width, uint32_t height)
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

	int cubeFaceWidth = width;
	int cubeFaceHeight = height;
	int cubeFacePositions[][2] = {
	  { cubeFaceWidth * 2, cubeFaceHeight }, // right, +X
	  { 0, cubeFaceHeight}, // left, -X
	  { cubeFaceWidth, 0 }, // top, +Y
	  { cubeFaceWidth, cubeFaceHeight * 2 }, // bottom, -Y
	  // inverted z axis?!
	  { cubeFaceWidth, cubeFaceHeight }, // front, -Z
	  { cubeFaceWidth * 3, cubeFaceHeight }, // back, +Z
	};

	std::vector<VkBufferImageCopy> bufferImageCopies;
	uint32_t offset = 0;
	for (int i = 0; i < 6; i++)
	{
		VkBufferImageCopy bufferImageCopy = {};
		bufferImageCopy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		bufferImageCopy.imageSubresource.baseArrayLayer = i;
		bufferImageCopy.imageSubresource.layerCount = 1;
		bufferImageCopy.imageSubresource.mipLevel = 0;
		bufferImageCopy.imageExtent.width = cubeFaceWidth;
		bufferImageCopy.imageExtent.height = cubeFaceHeight;
		bufferImageCopy.imageExtent.depth = 1;
		offset = cubeFacePositions[i][0] + (cubeFacePositions[i][1]) * width * 4;
		bufferImageCopy.bufferOffset = offset * 4;
		bufferImageCopy.bufferRowLength = width * 4;
		bufferImageCopy.bufferImageHeight = height * 3;
		bufferImageCopies.push_back(bufferImageCopy);
	}

	VkImageSubresourceRange subresourceRange = {};
	subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	subresourceRange.baseMipLevel = 0;
	subresourceRange.levelCount = 1;
	subresourceRange.baseArrayLayer = 0;
	subresourceRange.layerCount = 6;

	VkImageMemoryBarrier firstBarrier = {};
	firstBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	firstBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	firstBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	firstBarrier.image = textureData.image;
	firstBarrier.subresourceRange = subresourceRange;
	firstBarrier.srcAccessMask = 0;
	firstBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

	VkImageMemoryBarrier secondBarrier = {};
	secondBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	secondBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	secondBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	secondBarrier.image = textureData.image;
	secondBarrier.subresourceRange = subresourceRange;
	secondBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	secondBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

	CommandBuffer commandBuffer;
	commandBuffer.CreateSingleShotBuffer(device, commandPool);

	vkCmdPipelineBarrier(commandBuffer.GetCommandBuffer(), VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &firstBarrier);
	vkCmdCopyBufferToImage(commandBuffer.GetCommandBuffer(), staging.stagingBuffer, textureData.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, static_cast<uint32_t>(bufferImageCopies.size()), bufferImageCopies.data());
	vkCmdPipelineBarrier(commandBuffer.GetCommandBuffer(), VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &secondBarrier);

	commandBuffer.submitSingleShotBuffer(device, commandPool, queue);
	vmaDestroyBuffer(allocator, staging.stagingBuffer, staging.stagingBufferAlloc);

	VkImageViewCreateInfo imageViewCreateInfo = {};
	imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageViewCreateInfo.image = textureData.image;
	imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
	imageViewCreateInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
	imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
	imageViewCreateInfo.subresourceRange.levelCount = 1;
	imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
	imageViewCreateInfo.subresourceRange.layerCount = 6;

	DEV_ASSERT(vkCreateImageView(device, &imageViewCreateInfo, nullptr, &textureData.imageView), "Texture", "Error creating image view for cube texture!");

	VkPhysicalDeviceFeatures features = {};
	vkGetPhysicalDeviceFeatures(physicDevice, &features);
	const VkBool32 isAnisotropySupported = features.samplerAnisotropy;

	VkPhysicalDeviceProperties properties = {};
	vkGetPhysicalDeviceProperties(physicDevice, &properties);
	const float maxAnisotropy = properties.limits.maxSamplerAnisotropy;

	VkSamplerCreateInfo samplerCreateInfo = {};
	samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
	samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
	samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerCreateInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerCreateInfo.compareEnable = VK_FALSE;
	samplerCreateInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerCreateInfo.mipLodBias = 0.f;
	samplerCreateInfo.minLod = 0.f;
	samplerCreateInfo.maxLod = 1.f;
	samplerCreateInfo.anisotropyEnable = isAnisotropySupported;
	samplerCreateInfo.maxAnisotropy = maxAnisotropy;

	DEV_ASSERT(vkCreateSampler(device, &samplerCreateInfo, nullptr, &textureData.sampler), "Texture", "Error creating texture sampler!");

	VkDescriptorSetAllocateInfo descriptorSetAllocatorInfo = {};
	descriptorSetAllocatorInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descriptorSetAllocatorInfo.descriptorPool = descriptorPool;
	descriptorSetAllocatorInfo.descriptorSetCount = 1;
	descriptorSetAllocatorInfo.pSetLayouts;

	DEV_ASSERT(vkAllocateDescriptorSets(device, &descriptorSetAllocatorInfo, &textureData.descriptorSet), "Texture", "Error allocating descriptor sets!");

	VkDescriptorImageInfo descriptorImageInfo = {};
	descriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	descriptorImageInfo.imageView = textureData.imageView;
	descriptorImageInfo.sampler = textureData.sampler;

	VkWriteDescriptorSet writeDescriptorSet = {};
	writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	writeDescriptorSet.dstSet = textureData.descriptorSet;
	writeDescriptorSet.dstBinding = 0;
	writeDescriptorSet.descriptorCount = 1;
	writeDescriptorSet.pImageInfo = &descriptorImageInfo;

	vkUpdateDescriptorSets(device, 1, &writeDescriptorSet, 0, nullptr);
}

void Texture::Cleanup(VmaAllocator allocator, VkDescriptorPool descriptorPool, VkDevice device, TextureData textureData)
{

	vkFreeDescriptorSets(device, descriptorPool, 1, &textureData.descriptorSet);
	vkDestroySampler(device, textureData.sampler, nullptr);
	vkDestroyImageView(device, textureData.imageView, nullptr);
	vmaDestroyImage(allocator, textureData.image, textureData.imageAlloc);
	DEV_LOG(TE_INFO, "Texture", "Texture deleted!");
}