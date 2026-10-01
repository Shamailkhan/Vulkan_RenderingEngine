#pragma once
#include <vulkan/vulkan.h>

class VulkanImage
{

public:
	VulkanImage() = default;
	~VulkanImage();



	VulkanImage(const VulkanImage&) = delete;
	VulkanImage& operator=(const VulkanImage&) = delete;

	// Movable
	// --------------------------------------------------------

	VulkanImage(VulkanImage&& other) noexcept;

	VulkanImage& operator=(VulkanImage&& other) noexcept;

	//creation and destruction 

	bool Create(VkDevice device, VkPhysicalDevice physical, uint32_t width, uint32_t height,
		VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memoryProperties);

	void Destroy();

	//getter 

	VkImage getHandler() const { return m_image; }

	inline VkDeviceMemory getMemeory() const { return m_memory; }

	VkFormat getFormat() const { return m_format; }
	uint32_t getHeight()const { return m_height; }
	uint32_t getWidth()const { return m_width; }


	bool isCreated() const{ return m_image != VK_NULL_HANDLE; }

private:
	// Finds appropriate Vulkan memory type

	uint32_t FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter,
		VkMemoryPropertyFlags properties) const;

private:
	VkDevice m_device;
	

	VkDeviceMemory m_memory = VK_NULL_HANDLE;

	VkImage m_image = VK_NULL_HANDLE;

	VkFormat m_format = VK_FORMAT_UNDEFINED;
	uint32_t m_width = 0;
	uint32_t m_height = 0;

};

