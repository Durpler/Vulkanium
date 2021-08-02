#ifndef FIRSTAPP_H
#define FIRSTAPP_H

#include "VulkaniumWindow.h"
#include "VulkaniumPipeline.h"
#include "VulkaniumDevice.h"
#include "VulkaniumSwapChain.h"

//std
#include <memory>

using namespace vulkanium; 

class FirstApp
{
public: 
	static constexpr int WIDTH = 1280; 
	static constexpr int HEIGHT = 720; 

	FirstApp(); 
	~FirstApp();

	FirstApp(const FirstApp& other) = delete; 
	FirstApp& operator=(const FirstApp& other) = delete; 

	void run(); 

private:

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
};



#endif 