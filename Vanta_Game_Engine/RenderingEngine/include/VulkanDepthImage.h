#pragma once
#include <VulkanImage.h>
#include <VulkanImageView.h>

#include <vulkan/vulkan.h>
class VulkanDepthImage
{

public:
	VulkanDepthImage() = default;
	~VulkanDepthImage();

	// No copying
	VulkanDepthImage(const VulkanDepthImage&) = delete;
	VulkanDepthImage& operator=(const VulkanDepthImage&) = delete;
	
	// Move support
	VulkanDepthImage(VulkanDepthImage&& other) noexcept;
	VulkanDepthImage& operator=(VulkanDepthImage&& other) noexcept;

	//Initialization 

	bool Initialization(VkPhysicalDevice physicalDevice, VkDevice device, VkExtent2D ImageExtent);

	void Destroy();
	// Getters
	VkImage GetImage() const { return m_depthImage.getHandler(); }
	VkImageView GetImageView() const { return m_depthImageView.GetHandle(); }

	VkDeviceMemory GetMemory() const { return m_depthImage.getMemeory(); }

	VkFormat GetFormat() const { return m_depthFormat; }

	VkExtent2D GetExtent() const { return m_extent; }

	bool IsInitialized() const { return m_initialized; }

private:

	bool FindDepthFormat();

	bool IsDepthFormatSupported(VkFormat format)const;



private:
	VkPhysicalDevice m_physicalDevice=VK_NULL_HANDLE;

	VkDevice m_device=VK_NULL_HANDLE;

	VulkanImage m_depthImage ;

	VulkanImageView m_depthImageView;

	VkFormat m_depthFormat = VK_FORMAT_UNDEFINED;

	VkExtent2D m_extent{};

	bool m_initialized = false;

};

