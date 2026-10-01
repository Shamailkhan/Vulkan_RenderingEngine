#pragma once
#include <vulkan/vulkan.h>


class VulkanImageView
{

public:

	VulkanImageView() = default;
	~VulkanImageView();


	// Non-copyable
	VulkanImageView(const VulkanImageView&) = delete;

	VulkanImageView& operator=(
		const VulkanImageView&
		) = delete;

	//moveable

	VulkanImageView(VulkanImageView&& other) noexcept;
	VulkanImageView& operator=(VulkanImageView&& other)  noexcept;

	// Creation / destruction

	bool Create(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags flags) ;

	void Destroy();

	inline VkImageView GetHandle() const { return m_ImageView; }

	bool IsCreated() const { return m_ImageView != VK_NULL_HANDLE; }


private: 
	VkDevice m_device = VK_NULL_HANDLE;
	VkImageView m_ImageView = VK_NULL_HANDLE;

};

