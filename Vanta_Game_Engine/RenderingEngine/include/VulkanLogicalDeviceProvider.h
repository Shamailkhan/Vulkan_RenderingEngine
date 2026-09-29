#include "VulkanPhysicalDeviceProvider.h"
#include <vulkan/vulkan.hpp>
#include <vector>



class VulkanLogicalDeviceProvider
{

public :
	VulkanLogicalDeviceProvider(VkPhysicalDevice physicalDevice, const QueueFamilyIndices& queueIndices);
	~VulkanLogicalDeviceProvider();

	bool Initiialize();

	void DestroyDevice();


	// Vulkan logical device

	VkDevice GetDevice() const;

	// Queues

	VkQueue GetGraphicQueue() const;
	VkQueue GetPresentQueue() const;
	VkQueue GetTransferQueue() const;
	VkQueue GetComputeQueue() const;

	bool isInitialized() const;

private:
	bool CreateLogicalDevice();

	std::vector<VkDeviceQueueCreateInfo> CreateQueueCreateInfo() const;
	// Required device extensions
	std::vector<const char*>
		GetRequiredDeviceExtensions() const;

private:


	VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;

	QueueFamilyIndices m_queueFamilyIndices;

	VkDevice m_device = VK_NULL_HANDLE;

	VkQueue m_graphicsQueue = VK_NULL_HANDLE;
	VkQueue m_presentQueue = VK_NULL_HANDLE;
	VkQueue m_computeQueue = VK_NULL_HANDLE;
	VkQueue m_transferQueue = VK_NULL_HANDLE;

	bool m_initialized = false;
};




