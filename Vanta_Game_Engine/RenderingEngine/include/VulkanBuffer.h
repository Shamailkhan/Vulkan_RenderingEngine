#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include<cstdint>
#include "VulkanLogicalDeviceProvider.h"
#include "VulkanPhysicalDeviceProvider.h"
//#include <cstdint> is a C++ standard library header that provides fixed-width integer types.


struct VulkanBufferDescription
{
	VkDeviceSize size = 0;
	VkBufferUsageFlags usage = 0;

	VkMemoryPropertyFlags memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	std::vector<uint32_t> queueFamilyIndices;

};

class  VulkanBuffer
{

	VulkanBuffer() = default;
	~VulkanBuffer();

	VulkanBuffer(const VulkanBuffer&) = delete;
	VulkanBuffer& operator=(const VulkanBuffer&) = delete;

	VulkanBuffer( VulkanBuffer&& other) noexcept;
	VulkanBuffer& operator=( VulkanBuffer&& other) noexcept;
	

	bool Create(
		const VulkanLogicalDeviceProvider& logicalDevice,
		const VulkanPhysicalDeviceProvider& physicalDevice,
		const VulkanBufferDescription& description
	);
	void Destroy();

	void Unmap();

	bool Write(const void* data, VkDeviceSize size,
		VkDeviceSize offset = 0);

	VkBuffer getHandler() const { return m_buffer;}
	VkDeviceMemory GetMemory() const { return m_memory; }

	VkDeviceSize GetSize() const;
	VkDeviceSize getAllocationSize() const;

	VkBufferUsageFlags GetUsage() const;
	VkMemoryPropertyFlags GetMemoryProperties() const;

	bool isMapped()const;
	bool isCreated() const;

private:
	bool CreateBuffer(const VulkanBufferDescription& description);
	bool AllocateMemory(VkPhysicalDevice physicalDevice, VkMemoryPropertyFlags properties);


	bool BindMemory();

	uint32_t FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter,
		VkMemoryPropertyFlags properties)const;
private:

	VkDevice m_device = VK_NULL_HANDLE;

	VkBuffer m_buffer = VK_NULL_HANDLE;

	VkDeviceSize m_size = 0;
	VkDeviceMemory m_memory = VK_NULL_HANDLE;

	VkDeviceSize m_allocationSize = 0;
	VkBufferUsageFlags m_usage = 0;
	VkMemoryPropertyFlags m_memoryProperties = 0;

	void* m_mappedMemory = nullptr;
	bool  m_initialized = false;


};

