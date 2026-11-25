#include "VulkanUploadManager.h"

#include "VulkanSingleTimeCommand.h"
#include <iostream>


bool VulkanUploadManager::Initialize(const VulkanLogicalDeviceProvider& logicalDevice, const VulkanPhysicalDeviceProvider& physicalDevice,
	VulkanCommandPool& commandPool)
{
	if (logicalDevice.GetDevice() == VK_NULL_HANDLE || physicalDevice.getPhysicalDevice() || VK_NULL_HANDLE
		|| logicalDevice.GetGraphicQueue() == VK_NULL_HANDLE)
	{
		std::cerr << "VulkanUploadManager: Invalid providers.\n";
		return false;
	}
	
	if (!commandPool.InInitialized())
	{
		std::cerr << "VulkanUploadManager: Command pool not initialized.\n";
		return false;

	}


	if (commandPool.getDevice() != logicalDevice.GetDevice())
	{
		std::cerr << "VulkanUploadManager: Command pool belongs to "
			"a different logical device.\n";
		return false;
	}

	m_logicalDevice = &logicalDevice;
	m_physicalDevice = &physicalDevice;
	m_commandPool = &commandPool;
	Initialize = true;

	return true;

}

bool VulkanUploadManager::UploadBuffer(const void* sourceData, VkDeviceSize size, VulkanBuffer& destinationBuffer, VkDeviceSize destinationOffset)
{
	return false;
}

const bool VulkanUploadManager::ValidateUpload(const void* sourceData, VkDeviceSize size, const VulkanBuffer& destinationBuffer, VkDeviceSize destinationOffset) const
{
	return false;
}
