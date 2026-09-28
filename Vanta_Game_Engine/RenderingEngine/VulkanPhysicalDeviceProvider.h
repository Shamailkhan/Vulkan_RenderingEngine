#pragma once
#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>
#include <optional>
#include <cstdint>
// the queueFamily indices is important because a GPU can have more than one queue Families

struct QueueFamilyIndices
{
	//std::optional is used when a variable may or may not conytain a value
	std::optional<uint32_t> graphicFamily;
	std::optional<uint32_t> presentFamily;

	std::optional<uint32_t> computeFamily;
	std::optional<uint32_t> transferFamily;

	bool isCompleted() const
	{
		return graphicFamily.has_value() && presentFamily.has_value();
	}

};

//Swap Chain Support Detail

struct SwapChainSupportDetail
{

	VkSurfaceCapabilitiesKHR capabilities{};

	std::vector<VkSurfaceFormatKHR> format;
	std::vector<VkPresentModeKHR> presentMode;


};

class VulkanPhysicalDeviceProvider
{

public:
	VulkanPhysicalDeviceProvider(VkInstance instance, VkSurfaceKHR surface);
	~VulkanPhysicalDeviceProvider();

	bool Initialize();
	//Queue Families

	QueueFamilyIndices GetQueueFamiliesIndices() const;

	//Device Information

	VkPhysicalDeviceProperties getPhysicalDevicePropertices() const;

	VkPhysicalDeviceFeatures getPhysicalDeviceFeature() const;

	VkPhysicalDeviceMemoryProperties getMemoryProperties() const;

	SwapChainSupportDetail getPhysicalDeviceSwapChainDetail() const;

	bool isInitialized() const;

private:
	// Physical device enumeration

	bool EnumeratePhysicalDevice();

	// select best physical device

	bool  ChooseBestPhysicalDevice(std::vector<VkPhysicalDevice> devices);


	// Check whether device is suitable

	bool CheckDeviceSuitable(VkPhysicalDevice device);

	QueueFamilyIndices FindDeviceQueueFamiliesIndices(VkPhysicalDevice device) const;

	bool CheckDeviceExtensionSupport(VkPhysicalDevice device) const;
	// Swapchain support
	SwapChainSupportDetail QuerySwapchainSupport(VkPhysicalDevice device) const;
	
	int RateDeviceSuitability(
		VkPhysicalDevice device
	)  ;

	const std::vector<const char*>&
		GetRequiredDeviceExtensions() const;
	
private :
	VkPhysicalDevice m_physicaldevice=VK_NULL_HANDLE;
	VkInstance m_instance = VK_NULL_HANDLE;
	VkSurfaceKHR m_surfaceinstance = VK_NULL_HANDLE;

	QueueFamilyIndices m_queueFamilyIndices;

	VkPhysicalDeviceProperties m_properties{};
	VkPhysicalDeviceFeatures m_features{};


	VkPhysicalDeviceMemoryProperties
		m_memoryProperties{};


	SwapChainSupportDetail m_swapchainSupport;

	
	bool m_initialized =false;


};

