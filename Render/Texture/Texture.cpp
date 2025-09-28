#include "Texture.h"

void Texture::LoadTexture(VmaAllocator allocator, VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkDescriptorPool descriptorPool, VkDescriptorSetLayout descriptorSetLayout, VkQueue queue, TextureData texData, std::string textureFilename,
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

	std::memcpy(dataToUpload, data, textureSize);
	vmaUnmapMemory(allocator, stagingAllocator);
	vmaFlushAllocation(allocator, stagingAllocator, 0, textureSize);

	stbi_image_free(data);

	VkTextureStagingBuffer textureStagingBuffer = { stagingBuffer, stagingAllocator };

	UploadTextureToGPU(allocator, device, physicalDevice, commandPool, queue, textureData, textureStagingBuffer, descriptorPool, descriptorSetLayout, texWidth, texHeight, generateMipmaps, mipmapLevels);
}

void Texture::LoadTexture(VmaAllocator allocator, VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkDescriptorPool descriptorPool, VkDescriptorSetLayout descriptorSetLayout, VkQueue queue, TextureData textureData, aiTexel* texelData, std::string fileName, int width, int height, bool generateMipmaps, bool flipImage)
{
	int texWidth;
	int texHeight;
	int numberOfChannels;
	uint32_t mipmapLevels = 1;

	stbi_set_flip_vertically_on_load(flipImage);

	unsigned char* data;
	if (height == 0)
	{
		data = stbi_load_from_memory(reinterpret_cast<unsigned char*>(texelData), width, &texWidth, &texHeight, &numberOfChannels, STBI_rgb_alpha);
	}
	else
	{
		data = stbi_load_from_memory(reinterpret_cast<unsigned char*>(texelData), width * height, &texWidth, &texHeight, &numberOfChannels, STBI_rgb_alpha);
	}

	if (generateMipmaps)
	{
		mipmapLevels += static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight))));
	}

	VkDeviceSize imageSize = texWidth * texHeight * 4;

	VkBufferCreateInfo stagingBufferCreateInfo = {};
	stagingBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	stagingBufferCreateInfo.size = imageSize;
	stagingBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

	VkBuffer stagingBuffer;
	VmaAllocation stagingBufferAllocation;

	VmaAllocationCreateInfo stagingAllocationCreateInfo = {};
	stagingAllocationCreateInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;

	DEV_ASSERT(vmaCreateBuffer(allocator, &stagingBufferCreateInfo, &stagingAllocationCreateInfo, &stagingBuffer, &stagingBufferAllocation, nullptr), "Texture", "Error creating staging buffer!");

	void* uploadData;
	DEV_ASSERT(vmaMapMemory(allocator, stagingBufferAllocation, &uploadData), "Texture", "Error mapping the texture!");

	std::memcpy(uploadData, data, imageSize);
	vmaUnmapMemory(allocator, stagingBufferAllocation);
	vmaFlushAllocation(allocator, stagingBufferAllocation, 0, imageSize);

	stbi_image_free(data);

	VkTextureStagingBuffer stagingData = { stagingBuffer, stagingBufferAllocation };

	UploadTextureToGPU(allocator, device, physicalDevice, commandPool, queue, textureData, stagingData, descriptorPool, descriptorSetLayout, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight), generateMipmaps, static_cast<uint32_t>(mipmapLevels));
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

void Texture::UploadTextureToGPU(VmaAllocator allocator, VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue queue, TextureData textureData, VkTextureStagingBuffer textureStagingBuffer, VkDescriptorPool descriptorPool, VkDescriptorSetLayout descriptorSetLayout, uint32_t width, uint32_t height, bool generateMipmaps, uint32_t mipmapLevels)
{
	VkImageCreateInfo imageCreateInfo = {};
	imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
	imageCreateInfo.extent.width = width;
	imageCreateInfo.extent.height = height;
	imageCreateInfo.extent.depth = 1.f;
	imageCreateInfo.mipLevels = mipmapLevels;
	imageCreateInfo.arrayLayers = 1;
	imageCreateInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
	imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;

	VmaAllocationCreateInfo allocationCreateInfo = {};
	allocationCreateInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

	DEV_ASSERT(vmaCreateImage(allocator, &imageCreateInfo, &allocationCreateInfo, &textureData.image, &textureData.imageAlloc, nullptr), "Texture", "Error creating texture image!");

	CommandBuffer uploadCommandBuffer;
	uploadCommandBuffer.CreateSingleShotBuffer(device, commandPool);

	VkImageSubresourceRange stagingSubreasourceRange = {};
	stagingSubreasourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	stagingSubreasourceRange.baseMipLevel = 0;
	stagingSubreasourceRange.levelCount = mipmapLevels;
	stagingSubreasourceRange.baseArrayLayer = 0;
	stagingSubreasourceRange.layerCount = 1;

	VkImageMemoryBarrier stagingImageMemoryBarrier = {};
	stagingImageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	stagingImageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	stagingImageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	stagingImageMemoryBarrier.image = textureData.image;
	stagingImageMemoryBarrier.subresourceRange = stagingSubreasourceRange;
	stagingImageMemoryBarrier.srcAccessMask = 0;
	stagingImageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

	VkExtent3D extend = {};
	extend.height = height;
	extend.width = width;
	extend.depth = 1.f;

	VkBufferImageCopy stagingBufferImageCopy = {};
	stagingBufferImageCopy.bufferOffset = 0;
	stagingBufferImageCopy.bufferRowLength = 0;
	stagingBufferImageCopy.bufferImageHeight = 0;
	stagingBufferImageCopy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	stagingBufferImageCopy.imageSubresource.mipLevel = 0;
	stagingBufferImageCopy.imageSubresource.baseArrayLayer = 0;
	stagingBufferImageCopy.imageSubresource.layerCount = 1;
	stagingBufferImageCopy.imageExtent = extend;

	VkImageMemoryBarrier shaderStagingImageMemoryBarrier = {};
	shaderStagingImageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	shaderStagingImageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	if (mipmapLevels > 1)
	{
		shaderStagingImageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	}
	else
	{
		shaderStagingImageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	}
	shaderStagingImageMemoryBarrier.image = textureData.image;
	shaderStagingImageMemoryBarrier.subresourceRange = stagingSubreasourceRange;
	shaderStagingImageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	shaderStagingImageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

	vkCmdPipelineBarrier(uploadCommandBuffer.GetCommandBuffer(), VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &shaderStagingImageMemoryBarrier);
	vkCmdCopyBufferToImage(uploadCommandBuffer.GetCommandBuffer(), textureStagingBuffer.stagingBuffer, textureData.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &stagingBufferImageCopy);
	vkCmdPipelineBarrier(uploadCommandBuffer.GetCommandBuffer(), VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &shaderStagingImageMemoryBarrier);

	if (generateMipmaps)
	{
		VkImageSubresourceRange blitImageSubresourceRange = {};
		blitImageSubresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blitImageSubresourceRange.baseMipLevel = 0;
		blitImageSubresourceRange.levelCount = 1;
		blitImageSubresourceRange.baseArrayLayer = 0;
		blitImageSubresourceRange.layerCount = 1;

		VkImageMemoryBarrier firstBarrier = {};
		firstBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		firstBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		firstBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		firstBarrier.image = textureData.image;
		firstBarrier.subresourceRange = blitImageSubresourceRange;
		firstBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		firstBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

		VkImageMemoryBarrier secondBarrier = {};
		secondBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		secondBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		secondBarrier.dstAccessMask = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		secondBarrier.image = textureData.image;
		secondBarrier.subresourceRange = blitImageSubresourceRange;
		secondBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		secondBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		VkImageBlit mipBlit = {};
		mipBlit.srcOffsets[0] = { 0, 0, 0 };
		mipBlit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		mipBlit.srcSubresource.baseArrayLayer = 0;
		mipBlit.srcSubresource.layerCount = 1;
		mipBlit.dstOffsets[0] = { 0, 0, 0 };
		mipBlit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		mipBlit.dstSubresource.baseArrayLayer = 0;
		mipBlit.dstSubresource.layerCount = 1;

		int32_t mipHeight = height;
		int32_t mipWidth = width;

		for (int mipLevel = 1; mipLevel < mipmapLevels; mipLevel++)
		{
			mipBlit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
			mipBlit.srcSubresource.mipLevel = mipLevel - 1;

			mipBlit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
			mipBlit.dstSubresource.mipLevel = mipLevel;

			firstBarrier.subresourceRange.baseMipLevel = mipLevel - 1;

			vkCmdPipelineBarrier(uploadCommandBuffer.GetCommandBuffer(), VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &firstBarrier);

			vkCmdBlitImage(uploadCommandBuffer.GetCommandBuffer(), textureData.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, textureData.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &mipBlit, VK_FILTER_LINEAR);

			secondBarrier.subresourceRange.baseMipLevel = mipLevel - 1;
			vkCmdPipelineBarrier(uploadCommandBuffer.GetCommandBuffer(), VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &secondBarrier);

			if (mipWidth > 1) {
				mipWidth /= 2;
			}
			if (mipHeight > 1) {
				mipHeight /= 2;
			}			
		}

		if (mipmapLevels > 1)
		{
			VkImageMemoryBarrier lastBarrier = {};
			lastBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			lastBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			lastBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			lastBarrier.image = textureData.image;
			lastBarrier.subresourceRange = blitImageSubresourceRange;
			lastBarrier.subresourceRange.baseMipLevel = mipmapLevels - 1;
			lastBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			lastBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			vkCmdPipelineBarrier(uploadCommandBuffer.GetCommandBuffer(), VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &lastBarrier);

			uploadCommandBuffer.submitSingleShotBuffer(device, commandPool, queue);
			vmaDestroyBuffer(allocator, textureStagingBuffer.stagingBuffer, textureStagingBuffer.stagingBufferAlloc);

			VkImageViewCreateInfo textureImageViewCreateInfo = {};
			textureImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			textureImageViewCreateInfo.image = textureData.image;
			textureImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			textureImageViewCreateInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
			textureImageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			textureImageViewCreateInfo.subresourceRange.baseMipLevel = 0;
			textureImageViewCreateInfo.subresourceRange.levelCount = mipmapLevels;
			textureImageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
			textureImageViewCreateInfo.subresourceRange.layerCount = 1;

			DEV_ASSERT(vkCreateImageView(device, &textureImageViewCreateInfo, nullptr, &textureData.imageView), "Texture", "Error creating the image view!");

			VkPhysicalDeviceFeatures features = {};
			vkGetPhysicalDeviceFeatures(physicalDevice, &features);
			const VkBool32 isAnisotropyAvailable = features.samplerAnisotropy;

			VkPhysicalDeviceProperties properties = {};
			vkGetPhysicalDeviceProperties(physicalDevice, &properties);
			const float maxAnisotropy = properties.limits.maxSamplerAnisotropy;

			VkSamplerCreateInfo textureSamplerCreateInfo = {};
			textureSamplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
			textureSamplerCreateInfo.magFilter = VK_FILTER_LINEAR;
			textureSamplerCreateInfo.minFilter = VK_FILTER_LINEAR;
			textureSamplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			textureSamplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			textureSamplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			textureSamplerCreateInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
			textureSamplerCreateInfo.unnormalizedCoordinates = VK_FALSE;
			textureSamplerCreateInfo.compareEnable = VK_FALSE;
			textureSamplerCreateInfo.compareOp = VK_COMPARE_OP_ALWAYS;
			textureSamplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
			textureSamplerCreateInfo.mipLodBias = 0.f;
			textureSamplerCreateInfo.minLod = 0.f;
			textureSamplerCreateInfo.maxLod = static_cast<float>(mipmapLevels);
			textureSamplerCreateInfo.anisotropyEnable = isAnisotropyAvailable;
			textureSamplerCreateInfo.maxAnisotropy = maxAnisotropy;

			DEV_ASSERT(vkCreateSampler(device, &textureSamplerCreateInfo, nullptr, &textureData.sampler), "Texture", "Error creating the texture sampler!");

			VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {};
			descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			descriptorSetAllocateInfo.descriptorPool = descriptorPool;
			descriptorSetAllocateInfo.descriptorSetCount = 1;
			descriptorSetAllocateInfo.pSetLayouts = &descriptorSetLayout;

			DEV_ASSERT(vkAllocateDescriptorSets(device, &descriptorSetAllocateInfo, &textureData.descriptorSet), "Texture", "Error creating the texture descriptor set!");

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
	}
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