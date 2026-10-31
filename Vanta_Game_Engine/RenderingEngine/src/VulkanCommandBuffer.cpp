#include "VulkanCommandBuffer.h"
#include <iostream>

VulkanCommandBuffer::VulkanCommandBuffer( VulkanCommandBuffer&& other) noexcept
{
	m_CommandBuffer = other.m_CommandBuffer;
	m_level = other.m_level;
	m_recording = other.m_recording;

	other.m_CommandBuffer = VK_NULL_HANDLE;
	other.m_level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	other.m_recording = false;
}

VulkanCommandBuffer& VulkanCommandBuffer::operator=( VulkanCommandBuffer&& other) noexcept
{
	if (this != &other)
	{
		m_CommandBuffer = other.m_CommandBuffer;
		m_level = other.m_level;
		m_recording = other.m_recording;

		other.m_CommandBuffer = VK_NULL_HANDLE;
		other.m_level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		other.m_recording = false;
	}

	return *this;
}

bool VulkanCommandBuffer::Begin(VkCommandBufferUsageFlags usageFlags)
{
	if (m_CommandBuffer == VK_NULL_HANDLE)
	{
		std::cout
			<< "VulkanCommandBuffer: Cannot begin "
			"unallocated command buffer.\n";

		return false;
	}
	if (m_recording)
	{
		std:: cout
			<< "VulkanCommandBuffer: Command buffer "
			"is already recording.\n";

		return false;
	}

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = usageFlags;
	VkResult result = vkBeginCommandBuffer(m_CommandBuffer, &beginInfo);


	if (result != VK_SUCCESS)
	{
		std::cout
			<< "VulkanCommandBuffer: Failed to begin "
			"command buffer. VkResult = "
			<< result
			<< '\n';

		return false;
	}
	m_recording = true;
	return true;
}

bool VulkanCommandBuffer::End()
{
	if (m_CommandBuffer == VK_NULL_HANDLE)
	{
		std::cerr
			<< "VulkanCommandBuffer: Cannot end "
			"unallocated command buffer.\n";

		return false;
	}

	if (!m_recording)
	{
		std::cerr
			<< "VulkanCommandBuffer: Command buffer "
			"is not recording.\n";

		return false;
	}

	VkResult result = vkEndCommandBuffer(m_CommandBuffer);

	if (result != VK_SUCCESS)
	{
		std::cerr
			<< "VulkanCommandBuffer: Failed to end "
			"command buffer. VkResult = "
			<< result
			<< '\n';

		return false;
	}
	m_recording = false;

	return true;
}

bool VulkanCommandBuffer::Reset(VkCommandBufferResetFlags flags = 0)
{
	if (m_CommandBuffer == VK_NULL_HANDLE)
	{
		std::cerr
			<< "VulkanCommandBuffer: Cannot reset "
			"unallocated command buffer.\n";

		return false;
	}

	if (m_recording)
	{
		std::cerr
			<< "VulkanCommandBuffer: Cannot reset "
			"a command buffer while recording.\n";

		return false;
	}

	VkResult result = vkResetCommandBuffer(m_CommandBuffer, flags);

	if (result != VK_SUCCESS)
	{
		std::cerr
			<< "VulkanCommandBuffer: Failed to reset "
			"command buffer. VkResult = "
			<< result
			<< '\n';

		return false;
	}

	return true;
}

void VulkanCommandBuffer::SetHandle(VkCommandBuffer commandBuffer, VkCommandBufferLevel level)
{
	m_CommandBuffer = commandBuffer;
	m_level = level;
	m_recording = false;
}

void VulkanCommandBuffer::Clear()
{

	m_CommandBuffer = VK_NULL_HANDLE;
	m_level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	m_recording = false;
}
