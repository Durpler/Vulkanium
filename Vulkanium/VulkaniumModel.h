#ifndef VULKANIUM_MODEL_H
#define VULKANIUM_MODEL_H 

#include "VulkaniumDevice.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// std
#include <vector>


namespace vulkanium
{
	class VulkaniumModel
	{
	public: 

		struct Vertex
		{
			glm::vec2 position;
			glm::vec3 color; 

			static std::vector<VkVertexInputBindingDescription> getBindingDescriptions(); 
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions(); 
		};

		VulkaniumModel(VulkaniumDevice& device, const std::vector<Vertex> &vertices); 
		~VulkaniumModel(); 

		VulkaniumModel(const VulkaniumModel& other) = delete; 
		VulkaniumModel& operator=(const VulkaniumModel& other) = delete; 

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);

	private: 
		void createVertexBuffers(const std::vector<Vertex>& vertices);

		VulkaniumDevice& m_vkDevice; 
		VkBuffer m_VertexBuffer;
		VkDeviceMemory m_VertexBufferMemory; 
		uint32_t m_nVertexCount; 
	};
}

#endif 