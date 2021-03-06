#include "VulkaniumPipeline.h"
#include "VulkaniumModel.h"
//std
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <cassert>

namespace vulkanium
{
	VulkaniumPipeline::VulkaniumPipeline(
		VulkaniumDevice& device, 
		const std::string& vertFilepath, 
		const std::string& fragFilepath,
		const PipelineConfigInfo& configInfo)
		: vkaniumDevice(device)
	{
		createGraphicsPipeline(vertFilepath, fragFilepath, configInfo);
	}

	VulkaniumPipeline::~VulkaniumPipeline()
	{
		vkDestroyShaderModule(vkaniumDevice.device(), vertShaderModule, nullptr);
		vkDestroyShaderModule(vkaniumDevice.device(), fragShaderModule, nullptr);
		vkDestroyPipeline(vkaniumDevice.device(), graphicsPipeline, nullptr);

	}
	
	
	void VulkaniumPipeline::defaulPipelineConfigInfo(PipelineConfigInfo& configInfo, uint32_t width, uint32_t height)
	{
		
		//PipelineConfigInfo configInfo{};
		// define input assembly -> defines how the input data is going to be assambled to be dispalyed
		configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE; 
		
		// define viewport
		configInfo.viewport.x = 0.0f; 
		configInfo.viewport.y = 0.0f; 
		configInfo.viewport.width = static_cast<float>(width); 
		configInfo.viewport.height = static_cast<float>(height); 
		configInfo.viewport.minDepth = 0.0f; 
		configInfo.viewport.maxDepth = 1.0f; 

		// define scissor
		configInfo.scissor.offset = { 0,0 };
		configInfo.scissor.extent = { width, height};

		//
		configInfo.viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		configInfo.viewportInfo.viewportCount = 1;
		configInfo.viewportInfo.pViewports = &configInfo.viewport;
		configInfo.viewportInfo.scissorCount = 1;
		configInfo.viewportInfo.pScissors = &configInfo.scissor;

		//rastarization stage setup
		configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
		configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
		configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
		configInfo.rasterizationInfo.lineWidth = 1.0f; 
		configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_NONE; 
		configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
		configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE; 
		configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f; 
		configInfo.rasterizationInfo.depthBiasClamp = 0.0f; 
		configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f; 

		//multisample stage setup
		configInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
		configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		configInfo.multisampleInfo.minSampleShading = 1.0f; 
		configInfo.multisampleInfo.pSampleMask = nullptr; 
		configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;
		configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE; 
		configInfo.multisampleInfo.flags = 0; 

		// color blend attachment setup
		configInfo.colorBlendAttachment.colorWriteMask =
			VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
			VK_COLOR_COMPONENT_A_BIT;
		configInfo.colorBlendAttachment.blendEnable = VK_FALSE;
		configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
		configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		// colorblend setup
		configInfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		configInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
		configInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;
		configInfo.colorBlendInfo.attachmentCount = 1; 
		configInfo.colorBlendInfo.pAttachments = &configInfo.colorBlendAttachment;
		configInfo.colorBlendInfo.blendConstants[0] = 0.0f; 
		configInfo.colorBlendInfo.blendConstants[1] = 0.0f;
		configInfo.colorBlendInfo.blendConstants[2] = 0.0f;
		configInfo.colorBlendInfo.blendConstants[3] = 0.0f;

		// depth stencil info creation
		configInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		configInfo.depthStencilInfo.depthBoundsTestEnable = VK_TRUE; 
		configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE; 
		configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
		configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
		configInfo.depthStencilInfo.minDepthBounds = 0.0f; 
		configInfo.depthStencilInfo.maxDepthBounds = 1.0f; 
		configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
		configInfo.depthStencilInfo.front = {}; 
		configInfo.depthStencilInfo.back = {};
	}

	void VulkaniumPipeline::bind(VkCommandBuffer commandBuffer)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
	}

	std::vector<char> VulkaniumPipeline::ReadFile(const std::string& filepath)
	{
		//std::ios::ate sets the filepointer to the end of the file
		std::ifstream file(filepath, std::ios::ate | std::ios::binary); 
		if (!file.is_open())
		{
			throw std::runtime_error("failed to open file: " + filepath);
		}

		size_t fileSize = static_cast<size_t>(file.tellg());

		std::vector<char> buffer(fileSize); 
		// return to the start
		file.seekg(0); 
		file.read(buffer.data(), fileSize);

		file.close(); 
		return buffer; 
	}


	void VulkaniumPipeline::createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo)
	{
		assert(configInfo.pipelineLayout != VK_NULL_HANDLE &&
			"Cannot create graphics pipeline: no pipelineLayout provided in configInfo");
		assert(configInfo.renderPass != VK_NULL_HANDLE &&
			"Cannot create graphics pipeline: no renderPass provied in configInfo");
		auto vertCode = ReadFile(vertFilepath); 
		auto fragCode = ReadFile(fragFilepath);

		std::cout << "Vertex Shader Code Size: " << vertCode.size() << std::endl; 
		std::cout << "Fragment Shader Code Size: " << vertCode.size() << std::endl; 

		createShaderModule(vertCode, &vertShaderModule);
		createShaderModule(fragCode, &fragShaderModule);

		// initialize shader-stage create info
		VkPipelineShaderStageCreateInfo shaderStages[2];

		// vertex shader stage
		shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shaderStages[0].module = vertShaderModule; 
		shaderStages[0].pName = "main";
		shaderStages[0].flags = 0;
		shaderStages[0].pNext = nullptr;
		shaderStages[0].pSpecializationInfo = nullptr;
		
		// fragment shader stage
		shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shaderStages[1].module = fragShaderModule; 
		shaderStages[1].pName = "main";
		shaderStages[1].flags = 0;
		shaderStages[1].pNext = nullptr;
		shaderStages[1].pSpecializationInfo = nullptr;


		auto bindingDescriptions = VulkaniumModel::Vertex::getBindingDescriptions(); 
		auto attributeDescriptions = VulkaniumModel::Vertex::getAttributeDescriptions();




		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size()); 
		vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
		vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data(); 



		// create actual graphics pipeline
		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2; 
		pipelineInfo.pStages = shaderStages; 
		pipelineInfo.pVertexInputState = &vertexInputInfo; 
		pipelineInfo.pInputAssemblyState = &configInfo.inputAssemblyInfo; 
		pipelineInfo.pViewportState = &configInfo.viewportInfo;
		pipelineInfo.pRasterizationState = &configInfo.rasterizationInfo; 
		pipelineInfo.pMultisampleState = &configInfo.multisampleInfo;
		pipelineInfo.pColorBlendState = &configInfo.colorBlendInfo; 
		pipelineInfo.pDepthStencilState = &configInfo.depthStencilInfo; 
		pipelineInfo.pDynamicState = nullptr; 

		pipelineInfo.layout = configInfo.pipelineLayout; 
		pipelineInfo.renderPass = configInfo.renderPass; 
		pipelineInfo.subpass = configInfo.subpass; 

		pipelineInfo.basePipelineIndex = -1; 
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; 

		if (vkCreateGraphicsPipelines(vkaniumDevice.device(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create graphics pipeline correctly");
		}
		
		


	}
	void VulkaniumPipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule)
	{
		VkShaderModuleCreateInfo createInfo{}; 
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO; 
		createInfo.codeSize = code.size(); 
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		if (vkCreateShaderModule(vkaniumDevice.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create shader module");
		}
	}
}