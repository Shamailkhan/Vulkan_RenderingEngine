#include "VulkanSwapchainProvider.h"
#include "GLFWWindow.h"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <iostream>
#include <limits>
#include <set>
VulkanSwapchainProvider::VulkanSwapchainProvider(VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface,
	const QueueFamilyIndices& queueFamilequeue, GLFWWindow& window)
	:m_physicalDevice(physicalDevice),
	m_Device(device),
	m_surface(surface),
	m_queueFamilyIndices(queueFamilequeue),
	m_window(&window)
{
}
VulkanSwapchainProvider::~VulkanSwapchainProvider()
{
	DestroySwapChain();
}

bool VulkanSwapchainProvider::Initialize()
{
	if (isInitialized )
	{
		std::cout
			<< "Swapchain is already initialized."
			<< std::endl;

		return true;
	}

	if (m_physicalDevice == VK_NULL_HANDLE)
	{
		std::cout
			<< "Swapchain initialization failed: "
			<< "invalid physical device."
			<< std::endl;

		return false;

	}


	if (m_Device == VK_NULL_HANDLE)
	{
		std::cout
			<< "Swapchain initialization failed: "
			<< "invalid logical device."
			<< std::endl;

		return false;
	}


	if (m_surface == VK_NULL_HANDLE)
	{
		std::cout
			<< "Swapchain initialization failed: "
			<< "invalid surface."
			<< std::endl;

		return false;
	}


	if (m_window == nullptr)
	{
		std::cout
			<< "Swapchain initialization failed: "
			<< "invalid window."
			<< std::endl;

		return false;
	}


	if (!m_queueFamilyIndices.isCompleted())
	{
		std::cout
			<< "Swapchain initialization failed: "
			<< "queue family indices are incomplete."
			<< std::endl;

		return false;
	}



	// --------------------------------------------------------
	// A minimized window can have a 0x0 framebuffer.
	// Do not create a swapchain in that state.
	// --------------------------------------------------------

	if (m_window->isMinimized())
	{
		std::cout
			<< "Window is minimized. "
			<< "Swapchain creation postponed."
			<< std::endl;

		return false;
	}


	// --------------------------------------------------------
	// Create swapchain
	// --------------------------------------------------------

	if (!CreateSwapChain())
	{
		return false;
	}

	if (!RetriveSwapChainImages())
	{
		DestroySwapChain();
		return false;
	}
	if (!CreateSwapChainImagesView())
	{
		DestroySwapChain();

		return false;
	}


	isInitialized = true;


	std::cout
		<< "Vulkan swapchain initialized successfully."
		<< std::endl;

}

bool VulkanSwapchainProvider::Recreate()
{
	if (m_Device == VK_NULL_HANDLE)
	{
		std::cerr
			<< "Cannot recreate swapchain: "
			<< "invalid logical device."
			<< std::endl;

		return false;
	}


	if (m_window == nullptr)
	{
		return false;
	}


	// --------------------------------------------------------
	// If minimized, wait until a valid framebuffer size exists.
	// --------------------------------------------------------

	if (m_window->isMinimized())
	{
		return false;
	}
	VkResult result = vkDeviceWaitIdle(m_Device);
	if (result != VK_SUCCESS)
	{
		std::cout
			<< "Failed to wait for device idle during "
			<< "swapchain recreation. "
			<< "VkResult = "
			<< result
			<< std::endl;

		return false;
	}

	DestroySwapChain();
	if (!CreateSwapChain())
	{
		return false;
	}

	if (!RetriveSwapChainImages())
	{
		return false;
	}
	if (!CreateSwapChainImagesView())
	{
		return false;
	}
	isInitialized = true;
	std::cout
		<< "Vulkan swapchain recreated successfully."
		<< std::endl;


	return true;
}

void VulkanSwapchainProvider::DestroySwapChain()
{

	DestroySwapChaiinImagesView();
	m_images.clear();


	if (m_Device != VK_NULL_HANDLE && m_SwapChain != VK_NULL_HANDLE)
	{
		vkDestroySwapchainKHR(m_Device, m_SwapChain, nullptr);
		m_SwapChain =
			VK_NULL_HANDLE;
	}
	m_imagesFormat =
		VK_FORMAT_UNDEFINED;

	imageExtend = {};

	isInitialized = false;
}

bool VulkanSwapchainProvider::CreateSwapChain()
{

	SwapChainSupportDetail supportDetail=QuerySwapChainSupport();

	if (supportDetail.format.empty())
	{
		std::cout
			<< "Cannot create swapchain: "
			<< "no surface formats available."
			<< std::endl;

		return false;
	}

	if (supportDetail.presentMode.empty())
	{
		std::cout
			<< "Cannot create swapchain: "
			<< "no present modes available."
			<< std::endl;

		return false;
	}

	if (supportDetail.capabilities.currentExtent.width == 0 ||
		supportDetail.capabilities.currentExtent.height == 0)
	{
		return false;
	}
	
	VkSurfaceFormatKHR surfaceFormat = ChooseSurfaceFormat(supportDetail.format);


	VkPresentModeKHR presentMode = ChoosePresentMode(supportDetail.presentMode);


	VkExtent2D extent = ChooseExtent(supportDetail.capabilities);

	if (extent.width == 0 ||
		extent.height == 0)
	{
		return false;
	}

	uint32_t imageCount = ChooseImageCount(supportDetail.capabilities);


	VkSwapchainCreateInfoKHR CreateInfo=CreateSwapChainCreateInfo(supportDetail);

	CreateInfo.minImageCount = imageCount;
	CreateInfo.imageFormat = surfaceFormat.format;

	CreateInfo.imageColorSpace = surfaceFormat.colorSpace;
	CreateInfo.imageExtent = extent;
	CreateInfo.presentMode = presentMode;


	VkResult result = vkCreateSwapchainKHR(m_Device,&CreateInfo,nullptr,&m_SwapChain);
	if (result != VK_SUCCESS)
	{
		std::cerr
			<< "Failed to create Vulkan swapchain. "
			<< "VkResult = "
			<< result
			<< std::endl;

		m_SwapChain =
			VK_NULL_HANDLE;

		return false;
	}

	// Store selected properties

	m_imagesFormat = surfaceFormat.format;
	imageExtend = extent;

	return true;
}

bool VulkanSwapchainProvider::RetriveSwapChainImages()
{
	if (m_SwapChain == VK_NULL_HANDLE)
	{
		return false;
	}

	uint32_t imageCount = 0;
	VkResult result = vkGetSwapchainImagesKHR(m_Device, m_SwapChain, &imageCount, nullptr);

	if (result != VK_SUCCESS ||
		imageCount == 0)
	{
		std::cerr
			<< "Failed to retrieve swapchain image count. "
			<< "VkResult = "
			<< result
			<< std::endl;

		return false;
	}
	m_images.resize(imageCount);

	result = vkGetSwapchainImagesKHR(m_Device, m_SwapChain, &imageCount, m_images.data());
	if (result != VK_SUCCESS)
	{
		std::cerr
			<< "Failed to retrieve swapchain images. "
			<< "VkResult = "
			<< result
			<< std::endl;

		m_images.clear();

		return false;
	}



	// Vulkan may return the actual number.
	m_images.resize(imageCount);
	return true;
}

bool VulkanSwapchainProvider::CreateSwapChainImagesView()
{
	if (m_images.empty())
	{
		return false;
	}
	m_imagesViews.clear();

	m_imagesViews.resize(m_images.size());
	for (size_t i = 0;
		i < m_images.size();
		++i)
	{

		if (!m_imagesViews[i].Create(
			m_Device,
			m_images[i],
			m_imagesFormat,
			VK_IMAGE_ASPECT_COLOR_BIT))

		{
			std::cerr
				<< "Failed to create swapchain image view "
				<< "for image "
				<< i
				<< "."
				<< std::endl;

			DestroySwapChaiinImagesView();

			return false;
		}

	}

	return true;
}

void VulkanSwapchainProvider::DestroySwapChaiinImagesView()
{

	for (VulkanImageView& imageView :
		m_imagesViews)
	{
		imageView.Destroy();
	}


	m_imagesViews.clear();

}

SwapChainSupportDetail VulkanSwapchainProvider::QuerySwapChainSupport() const
{
	SwapChainSupportDetail details{};


	VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice,m_surface,&details.capabilities);


	if (result != VK_SUCCESS)
	{
		std::cout
			<< "Failed to query surface capabilities. "
			<< "VkResult = "
			<< result
			<< std::endl;

		return details;
	}
	uint32_t formatCount = 0;
	result = vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &formatCount,nullptr);
	if (result != VK_SUCCESS)
	{
		std::cout
			<< "Failed to query surface format count. "
			<< "VkResult = "
			<< result
			<< std::endl;

		return details;
	}


	if (formatCount > 0)
	{
		details.format.reserve(formatCount);
		result = vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &formatCount, details.format.data());

		if (result != VK_SUCCESS)
		{
			details.format.clear();
			std::cout
				<< "Failed to query surface formats. "
				<< "VkResult = "
				<< result
				<< std::endl;

			return details;


		}
		details.format.resize(formatCount);
	}
	// Present modes

	uint32_t presentModeCount = 0;

	result = vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice,m_surface,&presentModeCount,nullptr);

	if (result != VK_SUCCESS)
	{
		std::cout
			<< "Failed to query present mode count. "
			<< "VkResult = "
			<< result
			<< std::endl;

		return details;

	}


	if (presentModeCount > 0)
	{
		details.presentMode.resize(presentModeCount);

		result = vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, m_surface, &presentModeCount, details.presentMode.data());


		if (result != VK_SUCCESS)
		{
			details.presentMode.clear();

			std::cerr
				<< "Failed to query present modes. "
				<< "VkResult = "
				<< result
				<< std::endl;

			return details;

		}

		details.presentMode.resize(presentModeCount);
	}
	return  details;

}

VkSurfaceFormatKHR VulkanSwapchainProvider::ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats) const
{
	for (const VkSurfaceFormatKHR& format : formats)
	{

		if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return format;
		
		}

	}

	return formats.front();
}

VkPresentModeKHR VulkanSwapchainProvider::ChoosePresentMode(const std::vector<VkPresentModeKHR>& presentModes) const
{
	for (const VkPresentModeKHR mode : presentModes)
	{
		if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
			return mode;
	}
	// FIFO is guaranteed to be available for a surface.
	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanSwapchainProvider::ChooseExtent(const VkSurfaceCapabilitiesKHR& capabilitties) const
{
	if (capabilitties.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return capabilitties.currentExtent;
	}
	// Otherwise use GLFW framebuffer dimensions.

	int width = 0;
	int height = 0;

	glfwGetFramebufferSize(m_window->getHandler(), &width, &height);
	VkExtent2D actualExtent{};

	actualExtent.width = static_cast<uint32_t>(std::max(width, 0));
	actualExtent.height = static_cast<uint32_t>(std::max(height, 0));
	// Clamp to surface-supported range.

	actualExtent.width = std::clamp(actualExtent.width, capabilitties.minImageExtent.width, capabilitties.maxImageExtent.width);
	actualExtent.height = std::clamp(actualExtent.height, capabilitties.minImageExtent.height, capabilitties.maxImageExtent.height);

	return actualExtent;

}

uint32_t VulkanSwapchainProvider::ChooseImageCount(const VkSurfaceCapabilitiesKHR& capabilities) const
{
	
	uint32_t imageCount = capabilities.minImageCount + 1;

	if(capabilities.maxImageCount != 0 && imageCount > capabilities.maxImageCount)
	{
		imageCount = capabilities.maxImageCount;
	}
	return imageCount;
}

VkCompositeAlphaFlagBitsKHR VulkanSwapchainProvider::ChooseCompositeAlpha(const VkSurfaceCapabilitiesKHR& capabilities) const
{
	const  VkCompositeAlphaFlagBitsKHR modes[] =
	{
		VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,//Treat the Vulkan window as completely opaque.//Anything behind the window isn't visible through it.
		VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,//This is used when the RGB color values in your swapchain image have already been multiplied by alpha.
		VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,//Vulkan image contains normal/unmultiplied RGB values:
		VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR//Let the native windowing system decide how alpha should behave.
	};
	for (VkCompositeAlphaFlagBitsKHR mode : modes)
	{
		if (capabilities.supportedCompositeAlpha & mode)
		{
			return mode;
		}
	}
	return VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
}

VkSwapchainCreateInfoKHR VulkanSwapchainProvider::CreateSwapChainCreateInfo(const SwapChainSupportDetail& supportDetail) const
{
	
	
	VkSurfaceFormatKHR surfaceFormat = ChooseSurfaceFormat(supportDetail.format);
	VkPresentModeKHR presentMode = ChoosePresentMode(supportDetail.presentMode);

	VkExtent2D extent = ChooseExtent(supportDetail.capabilities);
	
	uint32_t imageCount =
		ChooseImageCount(
			supportDetail.capabilities
		);

	
	
	
	VkSwapchainCreateInfoKHR SwapChianCreateInfo{};
	SwapChianCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	SwapChianCreateInfo.pNext = nullptr;
	SwapChianCreateInfo.flags = 0;
	SwapChianCreateInfo.surface = m_surface;
	SwapChianCreateInfo.minImageCount = imageCount;
	SwapChianCreateInfo.imageFormat = surfaceFormat.format;
	SwapChianCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
	SwapChianCreateInfo.imageExtent = extent;
	SwapChianCreateInfo.imageArrayLayers = 1;
	SwapChianCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	if (m_queueFamilyIndices.graphicFamily.value() == m_queueFamilyIndices.presentFamily.value())
	{
		SwapChianCreateInfo.imageSharingMode =
			VK_SHARING_MODE_EXCLUSIVE;

		SwapChianCreateInfo.queueFamilyIndexCount =
			0;

		SwapChianCreateInfo.pQueueFamilyIndices =
			nullptr;
	}
	else
	{
		SwapChianCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;

		uint32_t queueFamilyIndices[] =
		{
			m_queueFamilyIndices.graphicFamily.value(),
			m_queueFamilyIndices.presentFamily.value()
		};
		SwapChianCreateInfo.queueFamilyIndexCount = 2;
		SwapChianCreateInfo.pQueueFamilyIndices = nullptr;

	}
	SwapChianCreateInfo.preTransform =
		supportDetail.capabilities.currentTransform;


	SwapChianCreateInfo.compositeAlpha =
		ChooseCompositeAlpha(
			supportDetail.capabilities
		);


	SwapChianCreateInfo.presentMode =
		presentMode;

	SwapChianCreateInfo.clipped =
		VK_TRUE;

	SwapChianCreateInfo.oldSwapchain =
		VK_NULL_HANDLE;


	return SwapChianCreateInfo;
}
