#pragma once
#include "VulkanBuffer.h"
#include "VulkanCommandPool.h"

#include "VulkanPhysicalDeviceProvider.h"
#include "VulkanLogicalDeviceProvider.h"



class  VulkanUploadManager
{
public:
	VulkanUploadManager() = default;
	~VulkanUploadManager() = default;

	VulkanUploadManager(const VulkanUploadManager&) = delete;
	VulkanUploadManager& operator=(const VulkanUploadManager&) = delete;

	bool Initialize(const VulkanLogicalDeviceProvider& logicalDevice, const VulkanPhysicalDeviceProvider& physicalDevice,
		VulkanCommandPool& commandPool);

	bool UploadBuffer(const void* sourceData, VkDeviceSize size, VulkanBuffer& destinationBuffer,
		VkDeviceSize destinationOffset = 0);

	bool Initialized()const { return isInitialize; }
private:

	const bool ValidateUpload(const void* sourceData, VkDeviceSize size, const VulkanBuffer& destinationBuffer,
		VkDeviceSize destinationOffset)const;

private:
	const VulkanLogicalDeviceProvider* m_logicalDevice=nullptr;

	const VulkanPhysicalDeviceProvider* m_physicalDevice = nullptr;

	VulkanCommandPool* m_commandPool = nullptr;
	bool isInitialize = false;

};

