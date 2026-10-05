#include "VulkanDepthImage.h"
#include <array>
#include <limits>
#include <iostream>



VulkanDepthImage::~VulkanDepthImage()
{
	Destroy();
}

bool VulkanDepthImage::Initialization(VkPhysicalDevice physicalDevice, VkDevice device, VkExtent2D ImageExtent)
{
	if (m_initialized)
		return true;
	if (physicalDevice == VK_NULL_HANDLE)
	{
		std::cout
			<< "VulkanDepthImage::Initialize - "
			<< "invalid physical device.\n";

		return false;
	}

	if (device == VK_NULL_HANDLE)
	{
		std::cout
			<< "VulkanDepthImage::Initialize - "
			<< "invalid logical device.\n";

		return false;
	}

	if (ImageExtent.width == 0 || ImageExtent.height == 0)
	{
		std::cout<< "VulkanDepthImage::Initialize - "
			<< "invalid extent.\n";

		return false;
	}

	m_physicalDevice = physicalDevice;
	m_device = device;
	m_extent = ImageExtent; 
	// Find supported depth format

	if (!FindDepthFormat())
	{
		std::cout
			<< "VulkanDepthImage::Initialize - "
			<< "failed to find supported depth format.\n";

		Destroy();
		return false;
	}

	if (!CreatDepthImage())
	{
		std::cout
			<< "VulkanDepthImage::Initialize - "
			<< "failed to create depth image.\n";

		Destroy();
		return false;
	} 
	// Allocate memory
	if (!AllocateDepthImgaeMemory())
	{
		std::cout
			<< "VulkanDepthImage::Initialize - "
			<< "failed to allocate depth image memory.\n";

		Destroy();
		return false;
	}
	// Bind image to memory
	if (!BindDepthImage())
	{
		std::cout
			<< "VulkanDepthImage::Initialize - "
			<< "failed to bind depth image memory.\n";

		Destroy();
		return false;
	}
	if (!CreateDepthImageView())
	{
		std::cout
			<< "VulkanDepthImage::Initialize - "
			<< "failed to create depth image view.\n";

		Destroy();
		return false;
	}

	m_initialized = true;
	return true;

}

void VulkanDepthImage::Destroy()
{
	if (m_device == VK_NULL_HANDLE)
		return;

	if (m_depthImageView != VK_NULL_HANDLE)
	{
		vkDestroyImageView(m_device, m_depthImageView, nullptr);
		m_depthImageView = VK_NULL_HANDLE;
	}

	if (m_depthImage != VK_NULL_HANDLE)
	{
		vkDestroyImage(m_device, m_depthImage, nullptr);
		m_depthImage = VK_NULL_HANDLE;
	}
	if (m_depthImageMemory != VK_NULL_HANDLE)
	{
		vkFreeMemory(m_device, m_depthImageMemory, nullptr);
		m_depthImageMemory = VK_NULL_HANDLE;
	}

	m_depthFormat = VK_FORMAT_UNDEFINED;

	m_extent = {};

	m_physicalDevice = VK_NULL_HANDLE;

	m_device = VK_NULL_HANDLE;

	m_initialized = false;
}

bool VulkanDepthImage::FindDepthFormat()
{
	constexpr std::array<VkFormat, 3> depthFormats =
	{
		VK_FORMAT_D32_SFLOAT,
		VK_FORMAT_D32_SFLOAT_S8_UINT,
		VK_FORMAT_D24_UNORM_S8_UINT
	};

	for (VkFormat format : depthFormats)
	{
		if (IsDepthFormatSupported(format))
		{
			m_depthFormat = format;
			return true;
		}
	}
	return false;

}

bool VulkanDepthImage::IsDepthFormatSupported(VkFormat format) const
{
	VkFormatProperties properties{};
	vkGetPhysicalDeviceFormatProperties(m_physicalDevice, m_depthFormat, &properties);

	return (properties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) != 0;
}

bool VulkanDepthImage::CreatDepthImage()
{
	VkImageCreateInfo DepthCreateInfo= CreateDepthImageCreateInfo();

	VkResult result = vkCreateImage(m_device,&DepthCreateInfo,nullptr,&m_depthImage);

	if (result != VK_SUCCESS)
	{
		std::cout
			<< "VulkanDepthImage::CreateDepthImage - "
			<< "vkCreateImage failed. Error: "
			<< result
			<< '\n';

		m_depthImage = VK_NULL_HANDLE;

		return false;
	}

	return true;
}

VkImageCreateInfo VulkanDepthImage::CreateDepthImageCreateInfo()
{
	VkImageCreateInfo  CreateInfo{};

	CreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	CreateInfo.imageType = VK_IMAGE_TYPE_2D;
	CreateInfo.format = m_depthFormat;
	CreateInfo.extent.width = m_extent.width;
	CreateInfo.extent.height = m_extent.height;

	CreateInfo.extent.depth = 1;

	CreateInfo.mipLevels = 1;
	CreateInfo.arrayLayers = 1;
	CreateInfo.samples= VK_SAMPLE_COUNT_1_BIT;

	CreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	CreateInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	CreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	CreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	return CreateInfo;

}

bool VulkanDepthImage::AllocateDepthImgaeMemory()
{

	VkMemoryRequirements memoryRequirements = GetMemoryRequirmrnts();
	if (memoryRequirements.size == 0)
	{
		std::cerr
			<< "VulkanDepthImage::AllocateDepthImageMemory - "
			<< "invalid memory requirements.\n";

		return false;
	}
	uint32_t memoryType = FindMemoryType(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	if (memoryType == std::numeric_limits<uint32_t>::max())
	{
		std::cerr
			<< "VulkanDepthImage::AllocateDepthImageMemory - "
			<< "failed to find suitable memory type.\n";

		return false;
	}
	VkMemoryAllocateInfo allocationInfo{};
	allocationInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;

	allocationInfo.allocationSize = memoryRequirements.size;
	allocationInfo.memoryTypeIndex = memoryType;

	VkResult result = vkAllocateMemory(m_device, &allocationInfo, nullptr, &m_depthImageMemory);
	if (result != VK_SUCCESS)
	{
		std::cerr
			<< "VulkanDepthImage::AllocateDepthImageMemory - "
			<< "vkAllocateMemory failed. Error: "
			<< result
			<< '\n';

		m_depthImageMemory = VK_NULL_HANDLE;

		return false;
	}
	return true;
}

VkMemoryRequirements VulkanDepthImage::GetMemoryRequirmrnts() const
{
	VkMemoryRequirements memoryRequiremnts{};

	if (m_depthImage == VK_NULL_HANDLE)
		return memoryRequiremnts;

	vkGetImageMemoryRequirements(m_device, m_depthImage, &memoryRequiremnts);
	return memoryRequiremnts;
}

uint32_t VulkanDepthImage::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const
{
	VkPhysicalDeviceMemoryProperties memoryProperties{};

	vkGetPhysicalDeviceMemoryProperties(
		m_physicalDevice,
		&memoryProperties
	);
	for (uint32_t i = 0;
	i < memoryProperties.memoryTypeCount;
	++i)
	{
		bool typeSupported =
			(typeFilter & (1u << i)) != 0;

		bool propertiesSupported =
			(memoryProperties.memoryTypes[i].propertyFlags
				& properties)
			== properties;

		if (typeSupported && propertiesSupported)
		{
			return i;
		}
	}

	return std::numeric_limits<uint32_t>::max();

}

bool VulkanDepthImage::BindDepthImage()
{
	if (m_depthImage == VK_NULL_HANDLE)
		return false;

	if (m_depthImageMemory == VK_NULL_HANDLE)
		return false;

	VkResult result =
		vkBindImageMemory(
			m_device,
			m_depthImage,
			m_depthImageMemory,
			0
		);

	if (result != VK_SUCCESS)
	{
		std::cerr
			<< "VulkanDepthImage::BindDepthImageMemory - "
			<< "vkBindImageMemory failed. Error: "
			<< result
			<< '\n';

		return false;
	}

	return true;
}

bool VulkanDepthImage::CreateDepthImageView()
{
	VkImageViewCreateInfo createInfo{};

	createInfo.sType =
		VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;

	createInfo.image =
		m_depthImage;

	createInfo.viewType =
		VK_IMAGE_VIEW_TYPE_2D;

	createInfo.format =
		m_depthFormat;

	createInfo.components.r =
		VK_COMPONENT_SWIZZLE_IDENTITY;

	createInfo.components.g =
		VK_COMPONENT_SWIZZLE_IDENTITY;

	createInfo.components.b =
		VK_COMPONENT_SWIZZLE_IDENTITY;

	createInfo.components.a =
		VK_COMPONENT_SWIZZLE_IDENTITY;

	createInfo.subresourceRange.aspectMask =
		VK_IMAGE_ASPECT_DEPTH_BIT;

	createInfo.subresourceRange.baseMipLevel =
		0;

	createInfo.subresourceRange.levelCount =
		1;

	createInfo.subresourceRange.baseArrayLayer =
		0;

	createInfo.subresourceRange.layerCount =
		1;

	// Some depth formats also contain stencil.
	if (m_depthFormat == VK_FORMAT_D32_SFLOAT_S8_UINT ||
		m_depthFormat == VK_FORMAT_D24_UNORM_S8_UINT)
	{
		createInfo.subresourceRange.aspectMask |=
			VK_IMAGE_ASPECT_STENCIL_BIT;
	}

	VkResult result =
		vkCreateImageView(
			m_device,
			&createInfo,
			nullptr,
			&m_depthImageView
		);

	if (result != VK_SUCCESS)
	{
		std::cerr
			<< "VulkanDepthImage::CreateDepthImageView - "
			<< "vkCreateImageView failed. Error: "
			<< result
			<< '\n';

		m_depthImageView = VK_NULL_HANDLE;

		return false;
	}

	return true;
}
