#include "VulkanImageView.h"
#include <iostream>
VulkanImageView::~VulkanImageView()
{
	Destroy();
}

VulkanImageView::VulkanImageView(VulkanImageView&& other) noexcept
	: m_device(other.m_device),
	m_ImageView(other.m_ImageView)
{
	other.m_device = VK_NULL_HANDLE;

	other.m_ImageView = VK_NULL_HANDLE;
}
VulkanImageView& VulkanImageView::operator=(VulkanImageView&& other)  noexcept
{
	if (this == &other)
		return *this;


	Destroy();


	m_device =
		other.m_device;

	m_ImageView =
		other.m_ImageView;


	other.m_device =
		VK_NULL_HANDLE;

	other.m_ImageView =
		VK_NULL_HANDLE;


	return *this;
}


bool VulkanImageView::Create(VkDevice device, VkImage image, VkFormat format,
	VkImageAspectFlags flags)
{

	if (device== VK_NULL_HANDLE)
	{
		std::cout
			<< "VulkanImageView::Create failed: "
			<< "invalid VkDevice."
			<< std::endl;

		return false;
	}
	if (image == VK_NULL_HANDLE)
	{
		std::cout
			<< "VulkanImageView::Create failed: "
			<< "invalid VkImage."
			<< std::endl;

		return false;
	}
	//destory any already existing image view 

	Destroy();

	m_device = device;

	// VkImageViewCreateInfo

	VkImageViewCreateInfo imageViewCreateInfo{};
	imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageViewCreateInfo.pNext = nullptr;
	imageViewCreateInfo.flags = 0;
	imageViewCreateInfo.image = image;
	imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	imageViewCreateInfo.format = format;

	// --------------------------------------------------------
  // Component mapping
  // --------------------------------------------------------
  //
  // Identity mapping:
  //
  // R -> R
  // G -> G
  // B -> B
  // A -> A
  // --------------------------------------------------------

	imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;


	// Subresource range
	imageViewCreateInfo.subresourceRange.aspectMask = flags;
	imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
	imageViewCreateInfo.subresourceRange.levelCount = 1;
	imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
	imageViewCreateInfo.subresourceRange.layerCount = 1;
	// Create image view

	VkResult result = vkCreateImageView(m_device, &imageViewCreateInfo, nullptr, &m_ImageView);


	if (result != VK_SUCCESS)
	{
		std::cout
			<< "Failed to create VkImageView. "
			<< "VkResult = "
			<< result
			<< std::endl;

		m_ImageView = VK_NULL_HANDLE;

		m_device = VK_NULL_HANDLE;

		return false;
	}
	return true;

}
void VulkanImageView::Destroy()
{
	if (m_device == VK_NULL_HANDLE)
	{
		m_ImageView = VK_NULL_HANDLE;
		return;
	}

	if (m_ImageView != VK_NULL_HANDLE)
	{
		vkDestroyImageView(m_device,m_ImageView,nullptr);
		m_ImageView = VK_NULL_HANDLE;
	}
	m_device = VK_NULL_HANDLE;
}