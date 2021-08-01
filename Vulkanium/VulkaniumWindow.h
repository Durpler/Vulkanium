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

		bool shouldClose(); 
		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface); 

	private:

		void initWindow(); 

		const int m_nWidth; 
		const int m_nHeight; 

		// reference to the actual window 
		GLFWwindow* m_Window; 

		std::string m_sWindowTitle; 

	};
}

#endif