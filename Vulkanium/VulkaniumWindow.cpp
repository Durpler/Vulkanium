#include "VulkaniumWindow.h"
#include <stdexcept>

vulkanium::VulkaniumWindow::VulkaniumWindow(int width, int height, std::string name) :
	nWidth(width),
	nHeight(height),
	sWindowTitle(name)
{
	glfwInit(); 
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); 
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(nWidth, nHeight, sWindowTitle.c_str(), nullptr, nullptr);
}

vulkanium::VulkaniumWindow::~VulkaniumWindow()
{
	glfwDestroyWindow(window);
	glfwTerminate(); 
}

bool vulkanium::VulkaniumWindow::ShouldClose()
{
	return glfwWindowShouldClose(window); 
}

void vulkanium::VulkaniumWindow::CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
{
	if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create window surface");
	}
}

void vulkanium::VulkaniumWindow::InitWindow()
{
}
