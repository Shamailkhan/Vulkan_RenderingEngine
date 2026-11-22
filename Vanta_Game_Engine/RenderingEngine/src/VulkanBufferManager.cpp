#include "VulkanBufferManager.h"

VulkanBufferManager::~VulkanBufferManager()
{
}

bool VulkanBufferManager::Initialize(const VulkanLogicalDeviceProvider& logicalDevice, const VulkanPhysicalDeviceProvider& physicalDevice)
{
	return false;
}

VulkanBuffer* VulkanBufferManager::Create(const std::string& name, const VulkanBufferDescription& description)
{
	return nullptr;
}

VulkanBuffer* VulkanBufferManager::get(const std::string& name)
{
	return nullptr;
}
