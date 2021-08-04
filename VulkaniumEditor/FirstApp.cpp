#include "FirstApp.h"
#include <stdexcept>
#include <array>

FirstApp::FirstApp()
{
	loadModels(); 
	createPipelineLayout(); 
	createPipeline(); 
	createCommandBuffers();
}

FirstApp::~FirstApp()
{
	vkDestroyPipelineLayout(m_vkaniumDevice.device(),m_pipelineLayout,nullptr);

}

void FirstApp::run()
{
	while (!m_vkaniumWindow.shouldClose())
	{
		glfwPollEvents();
		drawFrame();
	}
	vkDeviceWaitIdle(m_vkaniumDevice.device());
}

void FirstApp::loadModels()
{
	std::vector<VulkaniumModel::Vertex> vertices
	{
		{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
		{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
	}; 

	m_vkaModel = std::make_unique<VulkaniumModel>(m_vkaniumDevice, vertices);
}

void FirstApp::createPipelineLayout()
{
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0; 
	pipelineLayoutInfo.pSetLayouts = nullptr; 
	// push constants is used to sent a small amount of data to buffer
	pipelineLayoutInfo.pushConstantRangeCount = 0; 
	pipelineLayoutInfo.pPushConstantRanges = nullptr; 
	if (vkCreatePipelineLayout(m_vkaniumDevice.device(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create pipeline layout");
	}
}

void FirstApp::createPipeline()
{
	PipelineConfigInfo pipelineConfig{};
	VulkaniumPipeline::defaulPipelineConfigInfo(
		pipelineConfig,
		m_vkaniumSwapChain.width(),
		m_vkaniumSwapChain.height());
	pipelineConfig.renderPass = m_vkaniumSwapChain.getRenderPass();
	pipelineConfig.pipelineLayout = m_pipelineLayout;
	m_vkaniumPipeline = std::make_unique<VulkaniumPipeline>(
		m_vkaniumDevice,
		"Shader\\simple_shader.vert.spv",
		"Shader\\simple_shader.frag.spv",
		pipelineConfig
		);
}

void FirstApp::createCommandBuffers()
{
	m_commandBuffers.resize(m_vkaniumSwapChain.imageCount());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = m_vkaniumDevice.getCommandPool();
	allocInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());

	if (vkAllocateCommandBuffers(m_vkaniumDevice.device(), &allocInfo, m_commandBuffers.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate command buffers");
	}

	for (int i = 0; i < m_commandBuffers.size(); ++i)
	{
		VkCommandBufferBeginInfo beginInfo{}; 
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(m_commandBuffers[i], &beginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to begin recording command buffer");
		}

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO; 
		renderPassInfo.renderPass = m_vkaniumSwapChain.getRenderPass();
		renderPassInfo.framebuffer = m_vkaniumSwapChain.getFrameBuffer(i);

		renderPassInfo.renderArea.offset = { 0,0 };
		renderPassInfo.renderArea.extent = m_vkaniumSwapChain.getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{ };
		clearValues[0].color = { 0.1f,0.1f ,0.1f ,0.1f };
		clearValues[1].depthStencil = { 1.0f,0 }; // this value would be ignored
		
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(m_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		m_vkaniumPipeline->bind(m_commandBuffers[i]); 
		m_vkaModel->bind(m_commandBuffers[i]);
		m_vkaModel->draw(m_commandBuffers[i]);

		vkCmdEndRenderPass(m_commandBuffers[i]); 
		if (vkEndCommandBuffer(m_commandBuffers[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to record command buffer!");
		}
	}
}

void FirstApp::drawFrame()
{
	uint32_t imageIndex; 
	auto result = m_vkaniumSwapChain.acquireNextImage(&imageIndex); 

	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("failed to acquire swap chain image"); 
	}

	result = m_vkaniumSwapChain.submitCommandBuffers(&m_commandBuffers[imageIndex], &imageIndex);

	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to present swap chain image");
	}

}
