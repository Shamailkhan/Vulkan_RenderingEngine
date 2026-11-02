#pragma once
#include <vulkan/vulkan.h>

class   VulkanCommandBuffer
{
public: 
	VulkanCommandBuffer() = default;
	~VulkanCommandBuffer()=default;

	VulkanCommandBuffer(const VulkanCommandBuffer&) = delete;
	VulkanCommandBuffer& operator=(const VulkanCommandBuffer&) = delete;

	VulkanCommandBuffer(VulkanCommandBuffer&& other) noexcept;
	VulkanCommandBuffer& operator=( VulkanCommandBuffer&& other) noexcept;

	bool Begin(VkCommandBufferUsageFlags usageFlags = 0);
	bool End();
	bool Reset(VkCommandBufferResetFlags flags=0);

	VkCommandBuffer getHandle() const { return m_CommandBuffer; }

	VkCommandBufferLevel getLevel()const { m_level; }

	bool InAllocated() { return m_CommandBuffer != VK_NULL_HANDLE; }
	bool isRecording() { return m_recording; }
	void SetHandle(VkCommandBuffer commandBuffer,
		VkCommandBufferLevel level);

	void Clear();
private:
	friend class VulkanCommandPool;
	friend class VulkanCommandBufferManager;

private:
	VkCommandBuffer m_CommandBuffer = VK_NULL_HANDLE;
	VkCommandBufferLevel m_level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	bool m_recording = false;

};

