#include "VulkaniumWindow.h"
#include <stdexcept>

vulkanium::VulkaniumWindow::VulkaniumWindow(int width, int height, std::string name) :
	m_nWidth(width),
	m_nHeight(height),
	m_sWindowTitle(name)
{
	glfwInit(); 
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); 
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	m_Window = glfwCreateWindow(m_nWidth, m_nHeight, m_sWindowTitle.c_str(), nullptr, nullptr);
}

vulkanium::VulkaniumWindow::~VulkaniumWindow()
{
	glfwDestroyWindow(m_Window);
	glfwTerminate(); 
}

bool vulkanium::VulkaniumWindow::shouldClose()
{
	return glfwWindowShouldClose(m_Window); 
}

void vulkanium::VulkaniumWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
{
	if (glfwCreateWindowSurface(instance, m_Window, nullptr, surface) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create window surface");
	}
}

void vulkanium::VulkaniumWindow::initWindow()
{
}
