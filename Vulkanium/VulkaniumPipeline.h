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
		PipelineConfigInfo() = default; 
		PipelineConfigInfo(const PipelineConfigInfo&) = delete;
		PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

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

		static void defaulPipelineConfigInfo(PipelineConfigInfo& configInfo,uint32_t width, uint32_t height);

		void bind(VkCommandBuffer commandBuffer);

	private:
		static std::vector<char> ReadFile(const std::string& filepath);

		void createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo);

		void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

		VulkaniumDevice& vkaniumDevice; 
		VkPipeline graphicsPipeline; 
		VkShaderModule vertShaderModule; 
		VkShaderModule fragShaderModule; 


	};
}

#endif