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

	
	VkPipelineShaderStageCreateInfo shaderStages[] = { vertexShaderStageInfo, fragmentShaderStageInfo };

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
	multisamplingStateInfo.rasterizationSamples;

	// Depth Stencil
	VkPipelineDepthStencilStateCreateInfo depthStencilStateInfo = {};
	depthStencilStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencilStateInfo.depthTestEnable = VK_TRUE;
	depthStencilStateInfo.depthWriteEnable = VK_TRUE;
	depthStencilStateInfo.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencilStateInfo.depthBoundsTestEnable = VK_FALSE;
	depthStencilStateInfo.stencilTestEnable = VK_FALSE;

	// Color Blending (First attach the color to blend)
	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask;
	colorBlendAttachment.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo colorBlendStateInfo = {};
	colorBlendStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	


	VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
	pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineCreateInfo.stageCount;
	pipelineCreateInfo.pStages;
	pipelineCreateInfo.pVertexInputState;
	pipelineCreateInfo.pInputAssemblyState;
	pipelineCreateInfo.pViewportState;
	pipelineCreateInfo.pRasterizationState;
	pipelineCreateInfo.pMultisampleState;
	pipelineCreateInfo.pDepthStencilState;
	pipelineCreateInfo.pColorBlendState;
	pipelineCreateInfo.pDynamicState;
	pipelineCreateInfo.layout;
	pipelineCreateInfo.renderPass = renderpass;
	pipelineCreateInfo.subpass = 0;
	pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;

	DEV_ASSERT(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &pipeline), "SkyboxPipeline", "Error creating the Skybox graphics pipeline!");

	// Destroy shader no longer need the objects
	vertexShader->Cleanup(device);
	fragmentShader->Cleanup(device);
}

void SkyboxPipeline::Cleanup(VkDevice device)
{
	vkDestroyPipeline(device, pipeline, nullptr);
	DEV_LOG(TE_INFO, "SkyboxPipeline", "Skybox pipeline deleted!");
}