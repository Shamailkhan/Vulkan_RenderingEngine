#include "VulkanImage.h"
#include <iostream>


VulkanImage::~VulkanImage()
{
	Destroy();
}
uint32_t VulkanImage::FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter,
	VkMemoryPropertyFlags properties) const
{

	VkPhysicalDeviceMemoryProperties memoryProperties{};

	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i)
	{

		const bool typeSupport = (typeFilter & (1u << i)) != 0;
		const bool propertiesSupported =
			(
				memoryProperties.memoryTypes[i].propertyFlags
				& properties
				) == properties;


		if (typeSupport && propertiesSupported)
		{
			return i;
		}
	}
	return UINT32_MAX;

}

VulkanImage::VulkanImage(VulkanImage&& other) noexcept
	: m_device(other.m_device),
	m_image(other.m_image),
	m_memory(other.m_memory),
	m_format(other.m_format),
	m_width(other.m_width),
	m_height(other.m_height)
{
	other.m_device = VK_NULL_HANDLE;
	other.m_image = VK_NULL_HANDLE;
	other.m_memory = VK_NULL_HANDLE;
	other.m_format = VK_FORMAT_UNDEFINED;
	other.m_width = 0;
	other.m_height = 0;
}

VulkanImage& VulkanImage::operator=(VulkanImage&& other) noexcept
{
	
	if (this == &other)
		return *this;

	Destroy();
	m_device = other.m_device;
	m_image = other.m_image;
	m_memory = other.m_memory;
	m_format = other.m_format;
	m_width = other.m_width;
	m_height = other.m_height;

	other.m_device = VK_NULL_HANDLE;
	other.m_image = VK_NULL_HANDLE;
	other.m_memory = VK_NULL_HANDLE;
	other.m_format = VK_FORMAT_UNDEFINED;
	other.m_width = 0;
	other.m_height = 0;

	return *this;



}

bool VulkanImage::Create(VkDevice device, VkPhysicalDevice physical, uint32_t width,
	uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, 
	VkMemoryPropertyFlags memoryProperties)
{

	if (device == VK_NULL_HANDLE)
	{
		std::cout
			<< "VulkanImage::Create failed: "
			<< "invalid VkDevice."
			<< std::endl;

		return false;
	}
	if (physical == VK_NULL_HANDLE)
	{
		std::cout
			<< "VulkanImage::Create failed: "
			<< "invalid VkPhysicalDevice."
			<< std::endl;

		return false;
	}

	if (width == 0 || height == 0)
	{

		std::cout << "Vulkan Image Creation Failed Image diamensions are zero" << std::endl;
		return false;
	}

	// Destroy previous image if one exists
	Destroy();
	m_device = device;

	m_format = format;

	m_width = width;

	m_height = height;

	VkImageCreateInfo imageCreateInfo{};
	imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageCreateInfo.pNext = nullptr;
	imageCreateInfo.flags = 0;

	imageCreateInfo.imageType =
		VK_IMAGE_TYPE_2D;
	imageCreateInfo.format = format;

	imageCreateInfo.extent.height = height;
	imageCreateInfo.extent.width = width;
	imageCreateInfo.extent.depth = 1;

	imageCreateInfo.mipLevels = 1;
	imageCreateInfo.arrayLayers = 1;
	imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageCreateInfo.tiling = tiling;
	imageCreateInfo.usage = usage;
	imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	imageCreateInfo.queueFamilyIndexCount =
		0;

	imageCreateInfo.pQueueFamilyIndices =
		nullptr;

	imageCreateInfo.initialLayout =VK_IMAGE_LAYOUT_UNDEFINED;

	VkResult result = vkCreateImage(m_device, &imageCreateInfo, nullptr,& m_image);

	if (result != VK_SUCCESS)
	{
		std::cout
			<< "Failed to create VkImage. "
			<< "VkResult = "
			<< result
			<< std::endl;

		m_image = VK_NULL_HANDLE;

		m_device = VK_NULL_HANDLE;

		m_format = VK_FORMAT_UNDEFINED;

		m_width = 0;

		m_height = 0;

		return false;
	}
	VkMemoryRequirements memoryRequirments{};

	vkGetImageMemoryRequirements(m_device, m_image, &memoryRequirments);

	uint32_t memoryTypeIndex =
		FindMemoryType(
			physical,
			memoryRequirments.memoryTypeBits,
			memoryProperties
		);

	if (memoryTypeIndex == UINT32_MAX)
	{
		std::cout
			<< "Failed to find suitable memory type "
			<< "for VkImage."
			<< std::endl;

		Destroy();

		return false;
	}
	// VkMemoryAllocateInfo

	VkMemoryAllocateInfo allocationInfo{};
	allocationInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocationInfo.allocationSize = memoryRequirments.size;
	allocationInfo.memoryTypeIndex = memoryTypeIndex;
	allocationInfo.pNext = nullptr;


	// Allocate memory

	result = vkAllocateMemory(m_device, &allocationInfo, nullptr, &m_memory);
	if (result != VK_SUCCESS)
	{
		std::cout
			<< "Failed to allocate image memory. "
			<< "VkResult = "
			<< result
			<< std::endl;

		Destroy();

		return false;
	}

	result = vkBindImageMemory(m_device, m_image, m_memory, 0);
	if (result != VK_SUCCESS)
	{
		std::cout
			<< "Failed to bind image memory. "
			<< "VkResult = "
			<< result
			<< std::endl;

		Destroy();

		return false;
	}

	return true;

}

void VulkanImage::Destroy()
{
	if (m_device == VK_NULL_HANDLE)
	{
		m_image = VK_NULL_HANDLE;
		m_memory = VK_NULL_HANDLE;
		return;
	}


	if (m_image != VK_NULL_HANDLE)
	{
		vkDestroyImage(m_device, m_image, nullptr);
		m_image = VK_NULL_HANDLE;
	}

	if (m_memory != VK_NULL_HANDLE)
	{
		vkFreeMemory(m_device, m_memory, nullptr);
		m_memory = VK_NULL_HANDLE;

	}
	m_device = VK_NULL_HANDLE;

	m_format = VK_FORMAT_UNDEFINED;

	m_width = 0;

	m_height = 0;
}


