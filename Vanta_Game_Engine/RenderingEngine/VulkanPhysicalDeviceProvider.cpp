#include "VulkanPhysicalDeviceProvider.h"
#include <algorithm>
#include <iostream>
#include <limits>
#include <set>
#include <string>

namespace
{
	const std::vector<const char*> REQUIRED_DEVICE_EXTENSION = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

}

VulkanPhysicalDeviceProvider::VulkanPhysicalDeviceProvider(VkInstance instance, VkSurfaceKHR surface)
{
	m_instance = instance;
	m_surfaceinstance = surface;
}

VulkanPhysicalDeviceProvider::~VulkanPhysicalDeviceProvider()
{
	m_physicaldevice = VK_NULL_HANDLE;
}

bool VulkanPhysicalDeviceProvider::Initialize()
{
	if (m_initialized)
		return true;
	if (m_instance == VK_NULL_HANDLE)
	{
		std::cout<< "Cannot initialize physical device provider: "
			<< "Vulkan instance is null.\n";

		return false;
	}

	if (m_surfaceinstance == VK_NULL_HANDLE)
	{
		std::cout<<"Cannot initialize physical device provider: "
			<< "Vulkan surface is null.\n";

		return false;
	}

	if (!EnumeratePhysicalDevice())
		return false;

	if (!ChooseBestPhysicalDevice())
		return false;

	// Cache selected device information

	vkGetPhysicalDeviceProperties(m_physicaldevice, &m_properties);

	vkGetPhysicalDeviceFeatures(m_physicaldevice, &m_features);

	vkGetPhysicalDeviceMemoryProperties(m_physicaldevice, &m_memoryProperties);

	m_queueFamilyIndices = FindDeviceQueueFamiliesIndices(m_physicaldevice);

	m_swapchainSupport = QuerySwapchainSupport(m_physicaldevice);
	m_initialized = true; 
	
	std::cout
		<< "Selected physical device: "
		<< m_properties.deviceName
		<< '\n';

	return true;
}

QueueFamilyIndices VulkanPhysicalDeviceProvider::GetQueueFamiliesIndices() const
{
	

	return QueueFamilyIndices();
}

VkPhysicalDeviceProperties VulkanPhysicalDeviceProvider::getPhysicalDevicePropertices() const
{
	return VkPhysicalDeviceProperties();
}

VkPhysicalDeviceFeatures VulkanPhysicalDeviceProvider::getPhysicalDeviceFeature() const
{
	return VkPhysicalDeviceFeatures();
}

VkPhysicalDeviceMemoryProperties VulkanPhysicalDeviceProvider::getMemoryProperties() const
{
	return VkPhysicalDeviceMemoryProperties();
}

SwapChainSupportDetail VulkanPhysicalDeviceProvider::getPhysicalDeviceSwapChainDetail() const
{
	return SwapChainSupportDetail();
}

bool VulkanPhysicalDeviceProvider::isInitialized() const
{
	return false;
}

bool VulkanPhysicalDeviceProvider::EnumeratePhysicalDevice()
{
	uint32_t DeviceCount = 0;

	VkResult result=vkEnumeratePhysicalDevices(m_instance,&DeviceCount,nullptr);

	if (result != VK_SUCCESS)
	{
		std::cout << "Failed to enumerate Vulkan physical devices. "
			<< "VkResult = "
			<< result
			<< '\n';
		return false;

	}
	if (DeviceCount == 0)
	{
		std::cout
			<< "No Vulkan physical devices were found.\n";

		return false;
	}
	std::vector<VkPhysicalDevice> devices(DeviceCount);

	result = vkEnumeratePhysicalDevices(m_instance, &DeviceCount, devices.data());

	if (result != VK_SUCCESS)
	{
		std::cout
			<< "Failed to retrieve Vulkan physical devices. "
			<< "VkResult = "
			<< result
			<< '\n';

		return false;
	}

	std::cout
		<< "Available Vulkan physical devices: "
		<< DeviceCount
		<< '\n';

	for (size_t i = 0; i < devices.size(); i++)
	{

		VkPhysicalDeviceProperties properties{};

		vkGetPhysicalDeviceProperties(devices[i], &properties);
		std::cout
			<< "  [" << i << "] "
			<< properties.deviceName
			<< '\n';

	}

	return true;
}

bool VulkanPhysicalDeviceProvider::ChooseBestPhysicalDevice()
{


	return false;
}

bool VulkanPhysicalDeviceProvider::CheckDeviceSuitable(VkPhysicalDevice device)
{
	return false;
}

QueueFamilyIndices VulkanPhysicalDeviceProvider::FindDeviceQueueFamiliesIndices(VkPhysicalDevice device) const
{
	return QueueFamilyIndices();
}

bool VulkanPhysicalDeviceProvider::CheckDeviceExtensionSupport(VkPhysicalDevice device) const
{
	return false;
}

SwapChainSupportDetail VulkanPhysicalDeviceProvider::QuerySwapchainSupport(VkPhysicalDevice device) const
{
	return SwapChainSupportDetail();
}

int VulkanPhysicalDeviceProvider::RateDeviceSuitability(VkPhysicalDevice device) const
{
	return 0;
}

const std::vector<const char*>& VulkanPhysicalDeviceProvider::GetRequiredDeviceExtensions() const
{
	// TODO: insert return statement here
}
