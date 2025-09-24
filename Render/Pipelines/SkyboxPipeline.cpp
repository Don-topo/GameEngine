#include "SkyboxPipeline.h"

void SkyboxPipeline::Initialization(VkDevice device, VkPipelineLayout pipelineLayout, VkRenderPass renderpass, std::string vertexShaderFilename, std::string fragmentShaderFilename)
{
	// Load Shaders
	std::unique_ptr<Shader> vertexShader = std::make_unique<Shader>();
	vertexShader->LoadShader(device, vertexShaderFilename);
	std::unique_ptr<Shader> fragmentShader = std::make_unique<Shader>();
	fragmentShader->LoadShader(device, fragmentShaderFilename);

	// Create pipeline shader stage info => Need vertex + fragment stage info

	// 1) Vertex Shader stage info
	VkPipelineShaderStageCreateInfo vertexShaderStageInfo = {};
	vertexShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertexShaderStageInfo.module = vertexShader->GetShader();
	vertexShaderStageInfo.pName = "main";

	// 2) Fragment Shader stage info
	VkPipelineShaderStageCreateInfo fragmentShaderStageInfo = {};
	fragmentShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragmentShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragmentShaderStageInfo.module = fragmentShader->GetShader();
	fragmentShaderStageInfo.pName = "main";
	
	std::vector<VkPipelineShaderStageCreateInfo> shaderStages = { vertexShaderStageInfo, fragmentShaderStageInfo };
	std::vector<VkVertexInputBindingDescription> vertexBindings = {{0, static_cast<uint32_t>(sizeof(SkyboxVertex)), VK_VERTEX_INPUT_RATE_VERTEX}};

	// Vertex input => vertexAttributes + vertexBindings
	VkVertexInputAttributeDescription attributeDescription = {};
	attributeDescription.binding = 0;
	attributeDescription.location = 0;
	attributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescription.offset = static_cast<uint32_t>(offsetof(SkyboxVertex, position));

	// Vertex input
	VkPipelineVertexInputStateCreateInfo vertexInputStateInfo = {};
	vertexInputStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputStateInfo.vertexAttributeDescriptionCount = 1;
	vertexInputStateInfo.pVertexAttributeDescriptions = &attributeDescription;
	vertexInputStateInfo.vertexBindingDescriptionCount = vertexBindings.size();
	vertexInputStateInfo.pVertexBindingDescriptions = vertexBindings.data();

	// Input assembly
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateInfo = {};
	inputAssemblyStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyStateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssemblyStateInfo.primitiveRestartEnable = VK_FALSE;

	// Viewport
	VkViewport viewport{};
	VkRect2D scissor{};

	VkPipelineViewportStateCreateInfo viewportStateInfo = {};
	viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportStateInfo.viewportCount = 1;
	viewportStateInfo.pViewports = &viewport;
	viewportStateInfo.scissorCount = 1;
	viewportStateInfo.pScissors = &scissor;
	
	// Rasterizer
	VkPipelineRasterizationStateCreateInfo rasterizerStateInfo = {};
	rasterizerStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizerStateInfo.depthClampEnable = VK_FALSE;
	rasterizerStateInfo.rasterizerDiscardEnable = VK_FALSE;
	rasterizerStateInfo.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizerStateInfo.lineWidth = 1.0f;
	rasterizerStateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizerStateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizerStateInfo.depthBiasEnable = VK_FALSE;

	// Multisampling
	VkPipelineMultisampleStateCreateInfo multisamplingStateInfo = {};
	multisamplingStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisamplingStateInfo.sampleShadingEnable = VK_FALSE;
	multisamplingStateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	// Depth Stencil
	VkPipelineDepthStencilStateCreateInfo depthStencilStateInfo = {};
	depthStencilStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencilStateInfo.depthTestEnable = VK_FALSE;
	depthStencilStateInfo.depthWriteEnable = VK_FALSE;
	depthStencilStateInfo.depthCompareOp = VK_COMPARE_OP_ALWAYS;
	depthStencilStateInfo.depthBoundsTestEnable = VK_FALSE;
	depthStencilStateInfo.stencilTestEnable = VK_FALSE;
	depthStencilStateInfo.minDepthBounds = 0.f;
	depthStencilStateInfo.maxDepthBounds = 1.f;

	// Color Blending (First attach the color to blend)
	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo colorBlendStateInfo = {};
	colorBlendStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlendStateInfo.logicOpEnable = VK_FALSE;
	colorBlendStateInfo.logicOp = VK_LOGIC_OP_COPY;
	colorBlendStateInfo.attachmentCount = 1;
	colorBlendStateInfo.pAttachments = &colorBlendAttachment;
	colorBlendStateInfo.blendConstants[0] = 0.f;
	colorBlendStateInfo.blendConstants[1] = 0.f;
	colorBlendStateInfo.blendConstants[2] = 0.f;
	colorBlendStateInfo.blendConstants[3] = 0.f;

	// Dynamic state
	std::vector<VkDynamicState> dynamicStates = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};

	VkPipelineDynamicStateCreateInfo dynamicStateInfo = {};
	dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicStateInfo.dynamicStateCount = dynamicStates.size();
	dynamicStateInfo.pDynamicStates = dynamicStates.data();

	// Creation of the actual pipeline
	VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
	pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineCreateInfo.stageCount = 2;
	pipelineCreateInfo.pStages = shaderStages.data();
	pipelineCreateInfo.pVertexInputState = &vertexInputStateInfo;
	pipelineCreateInfo.pInputAssemblyState = &inputAssemblyStateInfo;
	pipelineCreateInfo.pViewportState = &viewportStateInfo;
	pipelineCreateInfo.pRasterizationState = &rasterizerStateInfo;
	pipelineCreateInfo.pMultisampleState = &multisamplingStateInfo;
	pipelineCreateInfo.pDepthStencilState = &depthStencilStateInfo;
	pipelineCreateInfo.pColorBlendState = &colorBlendStateInfo;
	pipelineCreateInfo.pDynamicState = &dynamicStateInfo;
	pipelineCreateInfo.layout = pipelineLayout;
	pipelineCreateInfo.renderPass = renderpass;
	pipelineCreateInfo.subpass = 0;
	pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;

	DEV_ASSERT(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &pipeline) == VK_SUCCESS, "SkyboxPipeline", "Error creating the Skybox graphics pipeline!");

	// Destroy shader no longer need the objects
	vertexShader->Cleanup(device);
	fragmentShader->Cleanup(device);
}

void SkyboxPipeline::Cleanup(VkDevice device)
{
	vkDestroyPipeline(device, pipeline, nullptr);
	DEV_LOG(TE_INFO, "SkyboxPipeline", "Skybox pipeline deleted!");
}