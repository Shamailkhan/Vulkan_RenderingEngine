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

	

	// Cache selected device information

	vkGetPhysicalDeviceProperties(m_physicaldevice, &m_properties);

	vkGetPhysicalDeviceFeatures(m_physicaldevice, &m_features);

	vkGetPhysicalDeviceMemoryProperties(m_physicaldevice, &m_memoryProperties);

//	m_queueFamilyIndices = FindDeviceQueueFamiliesIndices(m_physicaldevice);

	//m_swapchainSupport = QuerySwapchainSupport(m_physicaldevice);
	m_initialized = true; 
	
	std::cout
		<< "Selected physical device: "
		<< m_properties.deviceName
		<< '\n';

	return true;
}

QueueFamilyIndices VulkanPhysicalDeviceProvider::GetQueueFamiliesIndices() const
{
	

	return m_queueFamilyIndices;
}

VkPhysicalDeviceProperties VulkanPhysicalDeviceProvider::getPhysicalDevicePropertices() const
{
	return m_properties;
}

VkPhysicalDeviceFeatures VulkanPhysicalDeviceProvider::getPhysicalDeviceFeature() const
{
	return m_features;
}

VkPhysicalDeviceMemoryProperties VulkanPhysicalDeviceProvider::getMemoryProperties() const
{
	return m_memoryProperties;
}

SwapChainSupportDetail VulkanPhysicalDeviceProvider::getPhysicalDeviceSwapChainDetail() const
{
	return m_swapchainSupport;
}

bool VulkanPhysicalDeviceProvider::isInitialized() const
{
	return m_initialized;
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

	if (!ChooseBestPhysicalDevice(devices))
		return false;

	return true;
}

bool VulkanPhysicalDeviceProvider::ChooseBestPhysicalDevice(std::vector<VkPhysicalDevice> devices)
{
	//numerical limit min means Initialized best SCore to the Smallest value that a int can represent 

	int bestScore = std::numeric_limits<int>::min();

	VkPhysicalDevice bestDevice = VK_NULL_HANDLE;
	for (auto device : devices)
	{

		int score = RateDeviceSuitability(device);
		VkPhysicalDeviceProperties properties{};

		vkGetPhysicalDeviceProperties(device, &properties);

		std::cout
			<< "Device: "
			<< properties.deviceName
			<< " | Score: "
			<< score
			<< '\n';

		if (score > bestScore)
		{
			bestScore = score;

			bestDevice = device;

		}
		

	}
	if (bestDevice == VK_NULL_HANDLE)
	{
		std::cout << "Failed to find a suitable Vulkan physical device.\n";
		return false;
	}
	m_physicaldevice = bestDevice;
	m_queueFamilyIndices = FindDeviceQueueFamiliesIndices(m_physicaldevice);
	m_swapchainSupport = QuerySwapchainSupport(m_physicaldevice);

	return true;


}

bool VulkanPhysicalDeviceProvider::CheckDeviceSuitable(VkPhysicalDevice device)
{
	QueueFamilyIndices indices = FindDeviceQueueFamiliesIndices(device);
	// Required queue families

	if (!indices.isCompleted())
	{
		return false;
	}
	// Required device extensions
	if(!CheckDeviceExtensionSupport(device))
	{
		return false;
	}

	SwapChainSupportDetail swapchainSupport = QuerySwapchainSupport(device);

	if (swapchainSupport.format.empty() ||
		swapchainSupport.presentMode.empty())
	{
		return false;
	}
	// Device features
   // ------------------------------------------------------------

	VkPhysicalDeviceFeatures supportedFeatures{};


	vkGetPhysicalDeviceFeatures(
		device,
		&supportedFeatures
	);


	return true;
}

QueueFamilyIndices VulkanPhysicalDeviceProvider::FindDeviceQueueFamiliesIndices(VkPhysicalDevice device) const
{
	QueueFamilyIndices indices;
	uint32_t queueFamilyCount = 0;

	vkGetPhysicalDeviceQueueFamilyProperties(device,&queueFamilyCount,nullptr);

	if (queueFamilyCount == 0)
		return indices;
	
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);

	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	for (uint32_t i = 0;
		i < queueFamilyCount;
		++i)
	{
		const VkQueueFamilyProperties& queueFamily =
			queueFamilies[i];


		// --------------------------------------------------------
		// Graphics
		// --------------------------------------------------------

		if (queueFamily.queueCount > 0 &&
			(queueFamily.queueFlags &
				VK_QUEUE_GRAPHICS_BIT))
		{
			if (!indices.graphicFamily.has_value())
			{
				indices.graphicFamily = i;
			}
		}


		// --------------------------------------------------------
		// Compute
		// --------------------------------------------------------

		if (queueFamily.queueCount > 0 &&
			(queueFamily.queueFlags &
				VK_QUEUE_COMPUTE_BIT))
		{
			/*
				Prefer a dedicated compute queue if possible.
			*/

			bool dedicatedCompute =
				!(queueFamily.queueFlags &
					VK_QUEUE_GRAPHICS_BIT);


			if (!indices.computeFamily.has_value() ||
				dedicatedCompute)
			{
				indices.computeFamily = i;
			}
		}


		// --------------------------------------------------------
		// Transfer
		// --------------------------------------------------------

		if (queueFamily.queueCount > 0 &&
			(queueFamily.queueFlags &
				VK_QUEUE_TRANSFER_BIT))
		{
			/*
				Prefer a dedicated transfer queue.
			*/

			bool dedicatedTransfer =
				!(queueFamily.queueFlags &
					VK_QUEUE_GRAPHICS_BIT) &&
				!(queueFamily.queueFlags &
					VK_QUEUE_COMPUTE_BIT);


			if (!indices.transferFamily.has_value() ||
				dedicatedTransfer)
			{
				indices.transferFamily = i;
			}
		}
		// --------------------------------------------------------
	   // Present
	   // --------------------------------------------------------

		VkBool32 presentSupport = VK_FALSE;


		VkResult result =
			vkGetPhysicalDeviceSurfaceSupportKHR(
				device,
				i,
				m_surfaceinstance,
				&presentSupport
			);


		if (result == VK_SUCCESS &&
			presentSupport == VK_TRUE)
		{
			if (!indices.presentFamily.has_value())
			{
				indices.presentFamily = i;
			}
		}

	}


	return indices;
}

bool VulkanPhysicalDeviceProvider::CheckDeviceExtensionSupport(VkPhysicalDevice device) const
{
	uint32_t extensionCount = 0;

	VkResult result = vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	if (result != VK_SUCCESS)
		return false;

	std::vector<VkExtensionProperties> avalibleExtension(extensionCount);

	result =
		vkEnumerateDeviceExtensionProperties(
			device,
			nullptr,
			&extensionCount,
			avalibleExtension.data()
		);
	if (result != VK_SUCCESS)
		return false;

	std::set<std::string> requiredExtensions(
		REQUIRED_DEVICE_EXTENSION.begin(),
		REQUIRED_DEVICE_EXTENSION.end()
	);


	for (const auto& extension :
		avalibleExtension)
	{
		requiredExtensions.erase(
			extension.extensionName
		);
	}


	return requiredExtensions.empty();
	return false;
}

SwapChainSupportDetail VulkanPhysicalDeviceProvider::QuerySwapchainSupport(VkPhysicalDevice device) const
{
	 SwapChainSupportDetail detail;

	 // Surface capabilities
	 VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_surfaceinstance, &detail.capabilities);

	 if (result != VK_SUCCESS)
	 {
		 return detail;
	 }

	 // Surface formats
	 uint32_t formatCount = 0;
	 result =
		 vkGetPhysicalDeviceSurfaceFormatsKHR(
			 device,
			 m_surfaceinstance,
			 &formatCount,
			 nullptr
		 );


	 if (result != VK_SUCCESS)
	 {
		 return detail;
	 }
	 if (formatCount != 0)
	 {
		 detail.format.resize(formatCount);
		 result = vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surfaceinstance, &formatCount, detail.format.data());
		 if (result != VK_SUCCESS)
		 {
			 detail.format.clear();

			 return detail;
		 }
	 }

	 // Present modes
   // ------------------------------------------------------------

	 uint32_t presentModeCount = 0;


	 result =
		 vkGetPhysicalDeviceSurfacePresentModesKHR(
			 device,
			 m_surfaceinstance,
			 &presentModeCount,
			 nullptr
		 );


	 if (result != VK_SUCCESS)
	 {
		 return detail;
	 }


	 if (presentModeCount != 0)
	 {
		 detail.presentMode.resize(
			 presentModeCount
		 );


		 result =
			 vkGetPhysicalDeviceSurfacePresentModesKHR(
				 device,
				 m_surfaceinstance,
				 &presentModeCount,
				 detail.presentMode.data()
			 );


		 if (result != VK_SUCCESS)
		 {
			 detail.presentMode.clear();

			 return detail;
		 }
	 }
	 return detail;
}

int VulkanPhysicalDeviceProvider::RateDeviceSuitability(VkPhysicalDevice device) 
{

	//check weather the device is suitable 
	if (!CheckDeviceSuitable(device))
	{
		return -1;
	}

	VkPhysicalDeviceProperties properties{};
	VkPhysicalDeviceFeatures features{};
	vkGetPhysicalDeviceProperties(device, &properties);
	vkGetPhysicalDeviceFeatures(device, &features);

	int score = 0;

	// Prefer discrete GPUs

	if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
	{
		score += 500;
	}
	// More VRAM / larger heap is generally useful.
	//
	// This is deliberately not used as a hard requirement.
	VkPhysicalDeviceMemoryProperties memory_Properties{};

	vkGetPhysicalDeviceMemoryProperties(device, &memory_Properties);

	VkDeviceSize largestHeap = 0;
	for (uint32_t i = 0; i < memory_Properties.memoryHeapCount; i++)
	{
		if (memory_Properties.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
		{
			largestHeap = std::max(largestHeap, memory_Properties.memoryHeaps[i].size);

		}
	}
	// Rough scoring based on device-local memory.

	constexpr VkDeviceSize ONE_GB =
		1024ull * 1024ull * 1024ull;


	if (largestHeap >= 8 * ONE_GB)
	{
		score += 400;
	}
	else if (largestHeap >= 4 * ONE_GB)
	{
		score += 300;
	}
	else if (largestHeap >= 2 * ONE_GB)
	{
		score += 200;
	}
	else if (largestHeap >= ONE_GB)
	{
		score += 100;
	}


	// ------------------------------------------------------------
	// Geometry shader support
	// ------------------------------------------------------------

	if (features.geometryShader)
	{
		score += 50;
	}


	return score;

}

const std::vector<const char*>& VulkanPhysicalDeviceProvider::GetRequiredDeviceExtensions() const
{
	return REQUIRED_DEVICE_EXTENSION;

}
