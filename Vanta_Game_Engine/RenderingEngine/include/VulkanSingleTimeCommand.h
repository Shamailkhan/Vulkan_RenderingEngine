#pragma once
#include "VulkanCommandPool.h"
#include "VulkanCommandBuffer.h"
#include"vulkan/vulkan.h"


class   VulkanSingleTimeCommand
{

	VulkanSingleTimeCommand() = default;
	~VulkanSingleTimeCommand() = default;

	VulkanSingleTimeCommand(const VulkanSingleTimeCommand&) = delete;
	VulkanSingleTimeCommand& operator=(const VulkanSingleTimeCommand&) = delete;

	bool Begin(VkDevice device, VulkanCommandPool& commandPool);
	bool EndAndSubmit(VkQueue queue);
	VulkanCommandBuffer& GetCommandBuffer();
	const VulkanCommandBuffer& GetCommandBuffer()const;
	bool Active()const { return isActive; }

private:
	VkDevice m_device = VK_NULL_HANDLE;
	VulkanCommandPool* m_commandPool = nullptr;
	VulkanCommandBuffer m_Commandbuffer;

	bool isActive = false;


};

