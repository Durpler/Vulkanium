#ifndef VULKANIUM_PIPELINE_H
#define VULKANIUM_PIPELINE_H

#include "VulkaniumDevice.h"

// std
#include <string>
#include <vector>
namespace vulkanium
{
	// defines how this pipeline will make use of the graphics pipeline
	struct PipelineConfigInfo 
	{
		VkViewport viewport; 
		VkRect2D scissor; 
		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo; 
		VkPipelineMultisampleStateCreateInfo multisampleInfo; 
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo; 
		VkPipelineLayout pipelineLayout = nullptr; 
		VkRenderPass renderPass = nullptr; 
		uint32_t subpass = 0; 
	};


	class VulkaniumPipeline
	{
	public: 
		VulkaniumPipeline(
			VulkaniumDevice& device, 
			const std::string& vertFilepath, 
			const std::string& fragFilepath,
			const PipelineConfigInfo& configInfo);
		~VulkaniumPipeline(); 

		VulkaniumPipeline(const VulkaniumPipeline& other) = delete; 
		void operator=(const VulkaniumPipeline& other) = delete; 

		static PipelineConfigInfo defaulPipelineConfigInfo(uint32_t width, uint32_t height);

	private:
		static std::vector<char> ReadFile(const std::string& filepath);

		void CreateGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo);

		void CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

		VulkaniumDevice& vkaniumDevice; 
		VkPipeline graphicsPipeline; 
		VkShaderModule vertShaderModule; 
		VkShaderModule fragShaderModule; 


	};
}

#endif