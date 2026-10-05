#pragma once

#include <vulkan/vulkan.h>
class VulkanDepthImage
{

public:
	VulkanDepthImage() = default;
	~VulkanDepthImage();

	// No copying
	VulkanDepthImage(const VulkanDepthImage&) = delete;
	VulkanDepthImage& operator=(const VulkanDepthImage&) = delete;
	
	//Initialization 

	bool Initialization(VkPhysicalDevice physicalDevice, VkDevice device, VkExtent2D ImageExtent);

	void Destroy();
	// Getters
	VkImage GetImage() const { return m_depthImage; }
	VkImageView GetImageView() const { return m_depthImageView; }

	VkDeviceMemory GetMemory() const { return m_depthImageMemory; }

	VkFormat GetFormat() const { return m_depthFormat; }

	VkExtent2D GetExtent() const { return m_extent; }

	bool IsInitialized() const { return m_initialized; }

private:

	bool FindDepthFormat();

	bool IsDepthFormatSupported(VkFormat format)const;

	bool CreatDepthImage();

	VkImageCreateInfo CreateDepthImageCreateInfo();

	bool AllocateDepthImgaeMemory();

	VkMemoryRequirements GetMemoryRequirmrnts() const;
	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)const;

	bool BindDepthImage();

	bool CreateDepthImageView();

private:
	VkPhysicalDevice m_physicalDevice=VK_NULL_HANDLE;

	VkDevice m_device=VK_NULL_HANDLE;

	VkImage m_depthImage = VK_NULL_HANDLE;

	VkDeviceMemory m_depthImageMemory = VK_NULL_HANDLE;

	VkImageView m_depthImageView = VK_NULL_HANDLE;

	VkFormat m_depthFormat = VK_FORMAT_UNDEFINED;

	VkExtent2D m_extent{};

	bool m_initialized = false;

};

