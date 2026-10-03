#pragma once
#include "VulkanPhysicalDeviceProvider.h"
#include "VulkanImage.h"
#include "VulkanImageView.h"
#include <vulkan/vulkan.h>
#include <cstdint>
#include <vector>

class GLFWWindow;

class VulkanSwapchainProvider
{

	VulkanSwapchainProvider(VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface,
		const QueueFamilyIndices& queueFamilequeue, GLFWWindow& window);

	~VulkanSwapchainProvider();

	//Initialization


	bool Initialize();
	//swapchain recreation
	bool Recreate();

	//destruction

	void DestroySwapChain();

	//getter 
	VkSwapchainKHR getSwapChain();

	std::vector<VkImage>& getSwapChainImages()const;
	std::vector<VkImageView>& getSwapChainImageView() const;

	VkFormat getImageFormat() const;
	VkExtent2D getExtent()const;
	uint32_t getImageCount() const;
	bool isInitialized();

private:

	bool CreateSwapChain();

	bool RetriveSwapChainImages();

	bool CreateSwapChainImagesView();

	void DestroySwapChaiinImagesView();

	SwapChainSupportDetail QuerySwapChainSupport() const;

	VkSurfaceFormatKHR ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats)const;

	VkPresentModeKHR  ChoosePresentMode(const std::vector<VkPresentModeKHR>& presentModes) const;

	VkExtent2D ChooseExtent(const  VkSurfaceCapabilitiesKHR & capabilitties)const;

	uint32_t ChooseImageCount(const VkSurfaceCapabilitiesKHR& capabilities) const;
	
	VkCompositeAlphaFlagBitsKHR ChooseCompositeAlpha(const VkSurfaceCapabilitiesKHR& capabilities) const;


	VkSwapchainCreateInfoKHR CreateSwapChainCreateInfo(const SwapChainSupportDetail& supportDetail)const;


private:
	VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;

	VkDevice m_Device = VK_NULL_HANDLE;

	VkSurfaceKHR m_surface = VK_NULL_HANDLE;

	GLFWWindow* m_window =nullptr;

	QueueFamilyIndices m_queueFamilyIndices;


	VkSwapchainKHR m_SwapChain = VK_NULL_HANDLE;
	std::vector<VkImage> m_images;
	std::vector<VulkanImageView> m_imagesViews;

	VkFormat m_imagesFormat = VK_FORMAT_UNDEFINED;
	VkExtent2D imageExtend{};
	bool isInitialized = false;

};

