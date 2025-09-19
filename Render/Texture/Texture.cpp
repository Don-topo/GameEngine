#include "Texture.h"

void Texture::LoadCubeTexture()
{
	//stbi_set_flip_vertically_on_load();
}

void Texture::Cleanup(VkDevice device)
{
	// TODO Need descriptor pool and vma allocator
	//vkFreeDescriptorSets(device, );
	vkDestroySampler(device, textureData.sampler, nullptr);
	vkDestroyImageView(device, textureData.imageView, nullptr);
	//vmaDestroyImage(, textureData.image, textureData.imageAlloc);
	DEV_LOG(TE_INFO, "Texture", "Texture deleted!");
}