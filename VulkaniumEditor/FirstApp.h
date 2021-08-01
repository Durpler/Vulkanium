#ifndef FIRSTAPP_H
#define FIRSTAPP_H

#include "VulkaniumWindow.h"
#include "VulkaniumPipeline.h"
#include "VulkaniumDevice.h"

using namespace vulkanium; 

class FirstApp
{
public: 
	static constexpr int WIDTH = 1280; 
	static constexpr int HEIGHT = 720; 

	void run(); 

private:
	VulkaniumWindow vkaniumWindow{ WIDTH,HEIGHT,"Hello Vulkan!" }; 
	VulkaniumDevice vkaniumDevice{ vkaniumWindow }; 
	VulkaniumPipeline vkaniumPipeline{
		vkaniumDevice,
		"Shader\\simple_shader.vert.spv", 
		"Shader\\simple_shader.frag.spv" ,
		VulkaniumPipeline::defaulPipelineConfigInfo(WIDTH,HEIGHT)};
};



#endif 