#ifndef VULKANIUM_WINDOW_H
#define VULKANIUM_WINDOW_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace vulkanium
{
	class VulkaniumWindow
	{
	public: 
		VulkaniumWindow(int width, int height, std::string name);
		~VulkaniumWindow(); 

		VulkaniumWindow(const VulkaniumWindow& other) = delete; 
		VulkaniumWindow& operator=(const VulkaniumWindow& other) = delete; 

		bool ShouldClose(); 
		void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface); 

	private:

		void InitWindow(); 

		const int nWidth; 
		const int nHeight; 

		// reference to the actual window 
		GLFWwindow* window; 

		std::string sWindowTitle; 

	};
}

#endif