#ifndef FIRSTAPP_H
#define FIRSTAPP_H

#include "VulkaniumWindow.h"
#include "VulkaniumPipeline.h"
#include "VulkaniumDevice.h"
#include "VulkaniumSwapChain.h"
#include "VulkaniumModel.h"

//std
#include <memory>

using namespace vulkanium; 

class FirstApp
{
public: 
	static constexpr int WIDTH = 1024; 
	static constexpr int HEIGHT = 768; 

	FirstApp(); 
	~FirstApp();

	FirstApp(const FirstApp& other) = delete; 
	FirstApp& operator=(const FirstApp& other) = delete; 

	void run(); 

private:
	void loadModels(); 
	void createPipelineLayout();
	void createPipeline(); 
	void createCommandBuffers(); 
	void drawFrame();

	VulkaniumWindow m_vkaniumWindow{ WIDTH,HEIGHT,"Hello Vulkan!" }; 
	VulkaniumDevice m_vkaniumDevice{ m_vkaniumWindow }; 
	VulkaniumSwapChain m_vkaniumSwapChain{m_vkaniumDevice, m_vkaniumWindow.getExtent()};
	
	std::unique_ptr<VulkaniumPipeline> m_vkaniumPipeline;
	VkPipelineLayout m_pipelineLayout; 
	std::vector<VkCommandBuffer> m_commandBuffers; 

	std::unique_ptr<VulkaniumModel> m_vkaModel; 
};



#endif 