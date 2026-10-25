#pragma once
#include <vulkan/vulkan.h>
#include <cstdint>

class  VulkanCommandPool
{
public:
	VulkanCommandPool() = default;
	~VulkanCommandPool();

	VulkanCommandPool(const VulkanCommandPool&) = delete;
	VulkanCommandPool& operator=(const VulkanCommandPool&) = delete;


	VulkanCommandPool( VulkanCommandPool&& other ) noexcept;
	VulkanCommandPool& operator=(VulkanCommandPool&& other) noexcept;

	//Why VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT?
	//It allows you to independently reset command buffers :
	bool Initialize(VkDevice device, uint32_t queueFamilyIndex, 
		VkCommandPoolCreateFlags flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
	void Destroy();

	VkCommandPool GetHandle() const { return m_CommandPool; }
	VkDevice getDevice() const { return m_device; }
	uint32_t GetQueueFamilyIndex()const { return m_queueFamilyIndex; }

	bool InInitialized()const { return m_Initialize; }

private:
	bool CreateCommandPool(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags);
private:
	VkDevice m_device = VK_NULL_HANDLE;

	VkCommandPool m_CommandPool = VK_NULL_HANDLE;
	uint32_t m_queueFamilyIndex = 0;

	bool m_Initialize = false;

};

