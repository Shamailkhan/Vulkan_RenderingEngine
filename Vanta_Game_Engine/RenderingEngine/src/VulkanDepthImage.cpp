#include "VulkanDepthImage.h"
#include <array>
#include <limits>
#include <iostream>

VulkanDepthImage::VulkanDepthImage(
	VulkanDepthImage&& other
) noexcept
	:m_physicalDevice(other.m_physicalDevice),
	m_device(other.m_device),
	m_extent(other.m_extent),
	m_depthFormat(other.m_depthFormat),
	m_depthImage(std::move(other.m_depthImage)),
	m_depthImageView(std::move(other.m_depthImageView)),
	m_initialized(other.m_initialized)
{
	other.m_physicalDevice = VK_NULL_HANDLE;
	other.m_device = VK_NULL_HANDLE;
	other.m_extent = {};
	other.m_depthFormat = VK_FORMAT_UNDEFINED;
	other.m_initialized = false;
}
VulkanDepthImage& VulkanDepthImage::operator=(
	VulkanDepthImage&& other
	) noexcept
{
	if (this == &other)
		return *this;

	Destroy();

	m_physicalDevice = other.m_physicalDevice;
	m_device = other.m_device;
	m_extent = other.m_extent;
	m_depthFormat = other.m_depthFormat;

	m_depthImage = std::move(other.m_depthImage);
	m_depthImageView = std::move(other.m_depthImageView);

	m_initialized = other.m_initialized;

	other.m_physicalDevice = VK_NULL_HANDLE;
	other.m_device = VK_NULL_HANDLE;
	other.m_extent = {};
	other.m_depthFormat = VK_FORMAT_UNDEFINED;
	other.m_initialized = false;

	return *this;
}
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

	if (!m_depthImage.Create(m_device, m_physicalDevice, m_extent.width
		, m_extent.height, m_depthFormat,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT))
	{
		std::cout
			<< "VulkanDepthImage::Initialize - "
			<< "failed to create depth image.\n";

		Destroy();
		return false;
	} 
	// Allocate memory
	VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_DEPTH_BIT;
	if (m_depthFormat == VK_FORMAT_D32_SFLOAT_S8_UINT || m_depthFormat == VK_FORMAT_D24_UNORM_S8_UINT)
	{
		//|= is the bitwise OR assignment operator.
		aspectFlags |= VK_IMAGE_ASPECT_STENCIL_BIT;
	}

	if (!m_depthImageView.Create(m_device, m_depthImage.getHandler(), m_depthFormat, aspectFlags))
	{
		std::cout
			<< "VulkanDepthImage::Initialize - "
			<< "failed to allocate depth image memory.\n";

		Destroy();
		return false;
	}
	

	m_initialized = true;
	return true;

}

void VulkanDepthImage::Destroy()
{
	
	m_depthImageView.Destroy();
	m_depthImage.Destroy();
	m_physicalDevice = VK_NULL_HANDLE;
	m_device = VK_NULL_HANDLE;
	m_extent = {};
	m_depthFormat = VK_FORMAT_UNDEFINED;
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
	vkGetPhysicalDeviceFormatProperties(m_physicalDevice, format, &properties);

	return (properties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) != 0;
}
