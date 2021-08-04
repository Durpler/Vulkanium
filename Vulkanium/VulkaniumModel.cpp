#include "VulkaniumModel.h"
#include <cassert>

namespace vulkanium
{
	VulkaniumModel::VulkaniumModel(VulkaniumDevice& device, const std::vector<Vertex>& vertices)
		: m_vkDevice{ device }
	{
		createVertexBuffers(vertices);
	}

	VulkaniumModel::~VulkaniumModel()
	{
		// free up resources
		vkDestroyBuffer(m_vkDevice.device(), m_VertexBuffer, nullptr); 
		vkFreeMemory(m_vkDevice.device(), m_VertexBufferMemory, nullptr);
	}

	void VulkaniumModel::bind(VkCommandBuffer commandBuffer)
	{
		VkBuffer buffers[] = { m_VertexBuffer }; 
		VkDeviceSize offsets[] = { 0 }; 
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
	}

	void VulkaniumModel::draw(VkCommandBuffer commandBuffer)
	{
		vkCmdDraw(commandBuffer, m_nVertexCount, 1, 0, 0);
	}

	void VulkaniumModel::createVertexBuffers(const std::vector<Vertex>& vertices)
	{
		m_nVertexCount = static_cast<uint32_t>(vertices.size());
		assert(m_nVertexCount >= 3 && "Vertex count must be at least 3"); 
		VkDeviceSize bufferSize = sizeof(vertices[0]) * m_nVertexCount;

		m_vkDevice.createBuffer(bufferSize,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			m_VertexBuffer,
			m_VertexBufferMemory);

		void* data; 
		vkMapMemory(m_vkDevice.device(), m_VertexBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices.data(), static_cast<uint32_t>(bufferSize));
		vkUnmapMemory(m_vkDevice.device(), m_VertexBufferMemory); 
	}
	std::vector<VkVertexInputBindingDescription> VulkaniumModel::Vertex::getBindingDescriptions()
	{
		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
		bindingDescriptions[0].binding = 0; 
		bindingDescriptions[0].stride = sizeof(Vertex); 
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescriptions; 
	}
	std::vector<VkVertexInputAttributeDescription> VulkaniumModel::Vertex::getAttributeDescriptions()
	{
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions(1); 
		attributeDescriptions[0].binding = 0; 
		// corresponds to the location specified in the vertex shader
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].offset = 0; 
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT; 
		return attributeDescriptions;
	}
}