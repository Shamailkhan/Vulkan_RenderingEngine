#include "VulkanSwapchainProvider.h"
#include "GLFWWindow.h"
#include <GLFW/glfw3.h>

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
	return false;
}

void VulkanSwapchainProvider::DestroySwapChain()
{
}

bool VulkanSwapchainProvider::CreateSwapChain()
{
	return false;
}

bool VulkanSwapchainProvider::RetriveSwapChainImages()
{
	return false;
}

bool VulkanSwapchainProvider::CreateSwapChainImagesView()
{
	return false;
}

void VulkanSwapchainProvider::DestroySwapChaiinImagesView()
{
}

SwapChainSupportDetail VulkanSwapchainProvider::QuerySwapChainSupport() const
{
	return SwapChainSupportDetail();
}

VkSurfaceFormatKHR VulkanSwapchainProvider::ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats) const
{
	return VkSurfaceFormatKHR();
}

VkPresentModeKHR VulkanSwapchainProvider::ChoosePresentMode(const std::vector<VkPresentModeKHR>& presentModes)
{
	return VkPresentModeKHR();
}

VkExtent2D VulkanSwapchainProvider::ChooseExtent(const VkSurfaceCapabilitiesKHR& capabilitties) const
{
	return VkExtent2D();
}

uint32_t VulkanSwapchainProvider::ChooseImageCount(const VkSurfaceCapabilitiesKHR& capabilities) const
{
	return 0;
}

VkCompositeAlphaFlagBitsKHR VulkanSwapchainProvider::ChooseCompositeAlpha(const VkSurfaceCapabilitiesKHR& capabilities) const
{
	return VkCompositeAlphaFlagBitsKHR();
}

VkSwapchainCreateInfoKHR VulkanSwapchainProvider::CreateSwapChainCreateInfo(const SwapChainSupportDetail& supportDetail) const
{
	return VkSwapchainCreateInfoKHR();
}
