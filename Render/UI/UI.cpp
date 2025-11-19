#include "UI.h"

void UI::Initialization(VkDevice device, VkPhysicalDevice physicalDevice, vkb::Instance vkbInstance, VkQueue graphicsQueue, vkb::Swapchain swapchain, VkCommandPool commandPool, SDL_Window* window)
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
	
	commandBuffer.Initialization(device, commandPool);

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
	vulkanInfo.PipelineInfoMain.RenderPass = renderPass.GetRenderPass();
	vulkanInfo.PipelineInfoMain.Subpass = 0;
	vulkanInfo.PipelineInfoMain.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

	
	
	/*init_info.QueueFamily = g_QueueFamily;
	init_info.PipelineCache = g_PipelineCache;
	init_info.Allocator = g_Allocator;

	init_info.CheckVkResultFn = check_vk_result;*/


	DEV_ASSERT(ImGui_ImplVulkan_Init(&vulkanInfo), "UI", "Error initializating ImGui vulkan!");
	DEV_LOG(TE_INFO, "UI", "Vulkan ImGui Initializated!");

	ImGui::StyleColorsDark();
	DEV_LOG(TE_INFO, "UI", "UI Initializated!");
}

void UI::NewFrame()
{
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
}

void UI::CreateSettingsWindow()
{
	ImGui::SetNextWindowBgAlpha(0.8f);

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("New Config", "CTRL+N");
			ImGui::MenuItem("Load Config", "CTRL+L");
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if (ImGui::Begin("Scene", NULL)) {
		ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Add Component"))
			{
				if (ImGui::MenuItem("EmptyObject"))
				{
					//sceneManager->AddGameObject(GameObject());
				}
				if (ImGui::BeginMenu("Basic Objects"))
				{
					if (ImGui::MenuItem("Cube"))
					{
						//sceneManager->AddGameObject(GameObject("Cube"));
					}
					if (ImGui::MenuItem("Sphere"))
					{
						//sceneManager->AddGameObject(GameObject("Sphere"));
					}
					if (ImGui::MenuItem("Plane"))
					{
						//sceneManager->AddGameObject(GameObject("Plane"));
					}
					ImGui::EndMenu();
				}
				if (ImGui::MenuItem("Light"))
				{
					//sceneManager->AddGameObject(GameObject("Light"));
				}
				if (ImGui::MenuItem("Camera"))
				{
					//sceneManager->AddGameObject(GameObject("Camera"));
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::End();
	}

	if (ImGui::Begin("Properties", NULL))
	{
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnArrow))
		{
			static float x1 = 0.f;
			static float y1 = 0.f;
			static float z1 = 0.f;

			// Position
			ImGui::Text("Position");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
			// ImGui::DragFloat("X##1", &mSelectedGameObject.GetRenderObject()->GetModelData()->Position.x, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None);
			ImGui::DragFloat("X##1", &x1, 0.005f, -FLT_MAX, +FLT_MAX, "% .3f", ImGuiSliderFlags_None);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
			//ImGui::DragFloat("Y##1", &mSelectedGameObject.GetRenderObject()->GetModelData()->Position.y, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None);
			ImGui::DragFloat("Y##1", &y1, 0.005f, -FLT_MAX, +FLT_MAX, "% .3f", ImGuiSliderFlags_None);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
			//ImGui::DragFloat("Z##1", &mSelectedGameObject.GetRenderObject()->GetModelData()->Position.z, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None);
			ImGui::DragFloat("Z##1", &z1, 0.005f, -FLT_MAX, +FLT_MAX, "% .3f", ImGuiSliderFlags_None);
			// Rotation
			ImGui::Text("Rotation");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
			// ImGui::DragFloat("X##2", &mSelectedGameObject.GetRenderObject()->GetModelData()->Rotation.x, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None);
			ImGui::DragFloat("X##2", &x1, 0.005f, -FLT_MAX, +FLT_MAX, "% .3f", ImGuiSliderFlags_None);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
			//ImGui::DragFloat("Y##2", &mSelectedGameObject.GetRenderObject()->GetModelData()->Rotation.y, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None);
			ImGui::DragFloat("Y##2", &y1, 0.005f, -FLT_MAX, +FLT_MAX, "% .3f", ImGuiSliderFlags_None);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
			//ImGui::DragFloat("Z##2", &mSelectedGameObject.GetRenderObject()->GetModelData()->Rotation.z, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None);
			ImGui::DragFloat("Z##2", &z1, 0.005f, -FLT_MAX, +FLT_MAX, "% .3f", ImGuiSliderFlags_None);
			// Scale
			ImGui::Text("Scale   ");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
			// ImGui::DragFloat("X##3", &mSelectedGameObject.GetRenderObject()->GetModelData()->Scale.x, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None);
			ImGui::DragFloat("X##3", &x1, 0.005f, -FLT_MAX, +FLT_MAX, "% .3f", ImGuiSliderFlags_None);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
			// ImGui::DragFloat("Y##3", &mSelectedGameObject.GetRenderObject()->GetModelData()->Scale.y, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None);
			ImGui::DragFloat("Y##3", &y1, 0.005f, -FLT_MAX, +FLT_MAX, "% .3f", ImGuiSliderFlags_None);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
			// ImGui::DragFloat("Z##3", &mSelectedGameObject.GetRenderObject()->GetModelData()->Scale.z, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None);
			ImGui::DragFloat("Z##3", &z1, 0.005f, -FLT_MAX, +FLT_MAX, "% .3f", ImGuiSliderFlags_None);
			ImGui::TreePop();
		}
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::TreeNode("Renderer"))
		{
			/*ImGui::Checkbox("Enable render", mSelectedGameObject.GetRenderObject()->GetRenderIsEnabled());
			ImGui::Text("Mesh");
			ImGui::SameLine();

			if (mSelectedGameObject.GetRenderObject()->GetMesh().get()->GetMeshPath() == nullptr)
				ImGui::Text("drop here");
			else
			{
				ImGui::Text(mSelectedGameObject.GetRenderObject()->GetMesh().get()->GetMeshPath()->string().c_str());
				ImGui::SameLine();
				if (ImGui::SmallButton("Delete"))
				{
					// TODO Delete mesh
					mSelectedGameObject.GetRenderObject()->GetMesh();
				}
			}
			ImGui::Text("Color");
			ImGui::SameLine();
			if (mSelectedGameObject.GetRenderObject()->GetBaseColor().get()->GetTextureSourcePath() == nullptr)
				ImGui::Text("drop here");
			else
			{
				ImGui::Text(mSelectedGameObject.GetRenderObject()->GetBaseColor().get()->GetTextureSourcePath()->string().c_str());
				ImGui::SameLine();
				//ImGui::Text(mSelectedGameObject.GetRenderObject)
			}
			ImGui::Text("Emissive");
			ImGui::SameLine();
			ImGui::Text("Drop Here");
			ImGui::Text("Normal");
			ImGui::SameLine();
			ImGui::Text("Drop Here");
			*/
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Audio"))
		{
			static bool isAudioSource = false;

			ImGui::Text("Enabled?");
			ImGui::SameLine();
			ImGui::Checkbox("##enableAudio", &isAudioSource);
			ImGui::Text("Is Audio Source?");
			ImGui::SameLine();
			ImGui::Checkbox("##audioSource", &isAudioSource);
			ImGui::Text("Audio Clip");
			ImGui::SameLine();
			ImGui::TreePop();
		}
		ImGui::End();
	}

}

void UI::Draw()
{
	ImGui::Render();
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer.GetCommandBuffer());
}

void UI::PrepareCommandBuffer()
{
	commandBuffer.Reset();
	commandBuffer.BeginSingleShot();
}

void UI::BeginRenderPass(VkRenderPassBeginInfo renderPassBeginInfo)
{
	renderPassBeginInfo.renderPass = renderPass.GetRenderPass();
	vkCmdBeginRenderPass(commandBuffer.GetCommandBuffer(), &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void UI::EndRenderPass()
{
	vkCmdEndRenderPass(commandBuffer.GetCommandBuffer());
}

void UI::EndCommandBuffer()
{
	commandBuffer.End();
}

void UI::Cleanup(VkDevice device)
{
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	vkDestroyDescriptorPool(device, descriptorPool, nullptr);
	ImGui::DestroyContext();
	DEV_LOG(TE_INFO, "UI", "ImGui released!");
}

void UI::Test()
{
	DEV_LOG(TE_INFO, "UI", "Action callback");
}