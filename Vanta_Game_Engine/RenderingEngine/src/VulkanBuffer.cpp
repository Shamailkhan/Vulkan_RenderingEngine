#include "VulkanBuffer.h"
#include <iostream>
#include<cstring>
#include <utility>



VulkanBuffer::~VulkanBuffer()
{
	Destroy();
}


VulkanBuffer::VulkanBuffer( VulkanBuffer&& other) noexcept
{
	m_device = other.m_device;
	m_buffer = other.m_buffer;
	m_memory = other.m_memory;
	
	m_size = other.m_size;
	m_allocationSize = other.m_allocationSize;

	m_usage = other.m_usage;
	m_memoryProperties = other.m_memoryProperties;

	m_mappedMemory = other.m_mappedMemory;

	m_initialized = other.m_initialized;


	other.m_device = VK_NULL_HANDLE;
	other.m_buffer = VK_NULL_HANDLE;
	other.m_memory = VK_NULL_HANDLE;

	other.m_size = 0;
	other.m_allocationSize = 0;

	other.m_usage = 0;
	other.m_memoryProperties = 0;

	other.m_mappedMemory = nullptr;
	other.m_initialized = false;


}
VulkanBuffer& VulkanBuffer::operator=(VulkanBuffer&& other) noexcept
{
	if (this != &other)
	{
		Destroy();

		m_device = other.m_device;
		m_buffer = other.m_buffer;
		m_memory = other.m_memory;

		m_size = other.m_size;
		m_allocationSize = other.m_allocationSize;

		m_usage = other.m_usage;
		m_memoryProperties = other.m_memoryProperties;

		m_mappedMemory = other.m_mappedMemory;
		m_initialized = other.m_initialized;

		other.m_device = VK_NULL_HANDLE;
		other.m_buffer = VK_NULL_HANDLE;
		other.m_memory = VK_NULL_HANDLE;

		other.m_size = 0;
		other.m_allocationSize = 0;

		other.m_usage = 0;
		other.m_memoryProperties = 0;

		other.m_mappedMemory = nullptr;
		other.m_initialized = false;
	}

	return *this;

}
// const VulkanPhysicalDeviceProvider& physicalDevice this can only const functions 

bool VulkanBuffer::Create(const VulkanLogicalDeviceProvider& logicalDevice, const VulkanPhysicalDeviceProvider& physicalDevice, const VulkanBufferDescription& description)
{
	if (m_initialized || m_buffer != VK_NULL_HANDLE || m_memory != VK_NULL_HANDLE)
	{
		Destroy();

	}

	VkDevice device = logicalDevice.GetDevice();
	
	VkPhysicalDevice gpu= physicalDevice.getPhysicalDevice();

	if (device == VK_NULL_HANDLE || gpu == VK_NULL_HANDLE)
	{
		return false;
	}

	if (description.size == 0 ||
		description.usage == 0)
	{
		return false;
	}
	m_device = device;
	m_size = description.size;
	m_usage = description.usage;
	m_memoryProperties = description.memoryProperties;

	if (!CreateBuffer(description))
	{
		Destroy();
		return false;
	}



	if (!AllocateMemory(gpu, description.memoryProperties))
	{
		Destroy();
		return false;
	}

	if (!BindMemory())
	{
		Destroy();
		return false;
	}

	m_initialized = true;
	return true;
}


void VulkanBuffer::Destroy()
{
	Unmap();
	/*
	* Destroy buffer before freeing its bound memory.
	*/
	if (m_buffer != VK_NULL_HANDLE && m_device != VK_NULL_HANDLE)
	{
		vkDestroyBuffer(m_device, m_buffer, nullptr);
	}
	if (m_memory != VK_NULL_HANDLE && m_device != VK_NULL_HANDLE)
	{
		vkFreeMemory(m_device, m_memory, nullptr);

	}
	m_buffer = VK_NULL_HANDLE;
	m_memory = VK_NULL_HANDLE;
	m_device = VK_NULL_HANDLE;

	m_size = 0;
	m_allocationSize = 0;

	m_usage = 0;
	m_memoryProperties = 0;

	m_mappedMemory = nullptr;
	m_initialized = false;
}

void VulkanBuffer::Unmap()
{
}

bool VulkanBuffer::Write(const void* data, VkDeviceSize size, VkDeviceSize offset)
{
	return false;
}


bool VulkanBuffer::CreateBuffer(const VulkanBufferDescription& description)
{

	VkBufferCreateInfo createInfo{};

	createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	createInfo.size = description.size;
	createInfo.usage = description.usage;
	createInfo.sharingMode = description.sharingMode;

	if (description.sharingMode == VK_SHARING_MODE_CONCURRENT)
	{
		createInfo.queueFamilyIndexCount = static_cast<uint32_t>(description.queueFamilyIndices.size());

		createInfo.pQueueFamilyIndices = description.queueFamilyIndices.data();

	}

	VkResult result = vkCreateBuffer(m_device, &createInfo, nullptr, &m_buffer);
	if (result != VK_SUCCESS)
	{
		std::cerr
			<< "VulkanBuffer: Failed to create buffer. VkResult = "
			<< result << '\n';

		m_buffer = VK_NULL_HANDLE;

		return false;
	}

	return true;


}
bool VulkanBuffer::AllocateMemory(VkPhysicalDevice physicalDevice, VkMemoryPropertyFlags properties)
{

	VkMemoryRequirements MemoryRequirments{};


	vkGetBufferMemoryRequirements(m_device, m_buffer, &MemoryRequirments);

	m_allocationSize = MemoryRequirments.size;

	uint32_t memoryTypeIndex = FindMemoryType(physicalDevice, MemoryRequirments.memoryTypeBits, properties);

	if (memoryTypeIndex == UINT32_MAX)
	{
		std::cerr
			<< "VulkanBuffer: Failed to find suitable memory type.\n";

		return false;
	}

	VkMemoryAllocateInfo allocateInfo{};

	allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;

	allocateInfo.allocationSize = MemoryRequirments.size;
	allocateInfo.pNext = nullptr;
	allocateInfo.memoryTypeIndex = memoryTypeIndex;


	VkResult result = vkAllocateMemory(m_device, &allocateInfo, nullptr, &m_memory);

	if (result != VK_SUCCESS)
	{
		std::cerr
			<< "VulkanBuffer: Failed to allocate memory. VkResult = "
			<< result << '\n';

		m_memory = VK_NULL_HANDLE;

		return false;
	}
	return true;
}


bool VulkanBuffer::BindMemory()
{

	VkResult result = vkBindBufferMemory(m_device, m_buffer, m_memory, 0);


	if (result != VK_SUCCESS)
	{
		std::cerr
			<< "VulkanBuffer: Failed to bind memory. VkResult = "
			<< result << '\n';

		return false;
	}
	return true;

}

uint32_t VulkanBuffer::FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter,
	VkMemoryPropertyFlags properties)const
{

}





