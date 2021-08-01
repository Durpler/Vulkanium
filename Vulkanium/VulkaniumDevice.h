#ifndef VULKANIUM_DEVICE_H
#define VULKANIUM_DEVICE_H

#include "VulkaniumWindow.h"

// std lib headers
#include <string>
#include <vector>

namespace vulkanium
{

	struct SwapChainSupportDetails 
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	struct QueueFamilyIndices 
	{
		uint32_t graphicsFamily;
		uint32_t presentFamily;
		bool graphicsFamilyHasValue = false;
		bool presentFamilyHasValue = false;
		bool isComplete() { return graphicsFamilyHasValue && presentFamilyHasValue; }
	};

	class VulkaniumDevice
	{
	public:
#ifdef NDEBUG
		const bool enableValidationLayers = false;
#else
		const bool enableValidationLayers = true;
#endif

		VulkaniumDevice(VulkaniumWindow& window);
		~VulkaniumDevice();

		// Not copyable or movable
		VulkaniumDevice(const VulkaniumDevice&) = delete;
		void operator=(const VulkaniumDevice&) = delete;
		VulkaniumDevice(VulkaniumDevice&&) = delete;
		VulkaniumDevice& operator=(VulkaniumDevice&&) = delete;

		VkCommandPool getCommandPool() { return m_CommandPool; }
		VkDevice device() { return m_Device_; }
		VkSurfaceKHR surface() { return m_Surface_; }
		VkQueue graphicsQueue() { return m_GraphicsQueue_; }
		VkQueue presentQueue() { return m_PresentQueue_; }

		SwapChainSupportDetails getSwapChainSupport() { return querySwapChainSupport(m_PhysicalDevice); }
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		QueueFamilyIndices findPhysicalQueueFamilies() { return findQueueFamilies(m_PhysicalDevice); }
		VkFormat findSupportedFormat(
			const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

		// Buffer Helper Functions
		void createBuffer(
			VkDeviceSize size,
			VkBufferUsageFlags usage,
			VkMemoryPropertyFlags properties,
			VkBuffer& buffer,
			VkDeviceMemory& bufferMemory);
		VkCommandBuffer beginSingleTimeCommands();
		void endSingleTimeCommands(VkCommandBuffer commandBuffer);
		void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		void copyBufferToImage(
			VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount);

		void createImageWithInfo(
			const VkImageCreateInfo& imageInfo,
			VkMemoryPropertyFlags properties,
			VkImage& image,
			VkDeviceMemory& imageMemory);

		VkPhysicalDeviceProperties properties;

	private:
		void CreateInstance();
		void SetupDebugMessenger();
		void CreateSurface();
		void PickPhysicalDevice();
		void CreateLogicalDevice();
		void CreateCommandPool();

		// helper functions
		bool isDeviceSuitable(VkPhysicalDevice device);
		std::vector<const char*> getRequiredExtensions();
		bool checkValidationLayerSupport();
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void hasGflwRequiredInstanceExtensions();
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

		VkInstance m_Instance;
		VkDebugUtilsMessengerEXT m_DebugMessenger;
		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
		VulkaniumWindow& m_Window;
		VkCommandPool m_CommandPool;

		VkDevice m_Device_;
		VkSurfaceKHR m_Surface_;
		VkQueue m_GraphicsQueue_;
		VkQueue m_PresentQueue_;

		const std::vector<const char*> m_ValidationLayers_ = { "VK_LAYER_KHRONOS_validation" };
		const std::vector<const char*> m_DeviceExtensions_ = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	};

}  // namespace lve

#endif