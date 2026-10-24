#include "VulkanFramebufferManager.h"
#include <iostream>
#include <stdexcept>
#include <utility>
#include <vector>
VulkanFramebufferManager::~VulkanFramebufferManager()
{
	Destroy();

}


bool VulkanFramebufferManager::CreateFrame(VkDevice device, const VulkanRenderPass& renderpass, const std::vector<VulkanImageView>& swapChainImageView, const VulkanImageView& depthView, VkExtent2D extent)
{
	if (m_Initialize)
	{
		return true;
	}

	if (device == VK_NULL_HANDLE)
	{
		std::cout
			<< "VulkanFramebufferManager::Create - "
			"invalid VkDevice\n";

		return false;
	}

	if (!renderpass.IsInitialized())
	{
		std::cout
			<< "VulkanFramebufferManager::Create - "
			"render pass is not initialized\n";

		return false;
	}

	if (!swapChainImageView.empty())
	{
		std::cout
			<< "VulkanFramebufferManager::Create - "
			"swapchain image view list is empty\n";

		return false;
	}

	if (!depthView.IsCreated())
	{
		std::cout
			<< "VulkanFramebufferManager::Create - "
			"depth image view is not created\n";

		return false;

	}
	if(extent.width == 0 ||
		extent.height == 0)
	{
		std::cerr
			<< "VulkanFramebufferManager::Create - "
			"invalid framebuffer extent\n";

		return false;
	}/*
     * ------------------------------------------------------------
     * Create framebuffers in a temporary vector.
     *
     * This is important:
     *
     * If framebuffer #2 fails, we don't want to leave
     * the manager containing framebuffer #0 and #1.
     *
     * The temporary vector owns the successfully-created
     * framebuffers until the entire operation succeeds.
     * ------------------------------------------------------------
     */
	std::vector<VulkanFramebuffer> newFrameBuffer;
	newFrameBuffer.reserve(swapChainImageView.size());

	/*
	* ------------------------------------------------------------
	* One framebuffer per swapchain image view
	* ------------------------------------------------------------
	*/

	for (size_t i = 0; swapChainImageView.size(); ++i)
	{

		const VulkanImageView& swapChainView = swapChainImageView[i];

		if (!swapChainView.IsCreated())
		{
			std::cout
				<< "VulkanFramebufferManager::Create - "
				"invalid swapchain image view at index "
				<< i
				<< '\n';

			return false;
		}
		VulkanFramebuffer buffer;
		/*
		* The framebuffer does NOT take ownership of:
		*
		*     renderPass
		*     swapchainView
		*     depthView
		*
		* It only uses their Vulkan handles.
		*/
		if (!buffer.Initialize(device, renderpass, swapChainView, depthView, extent))
		{
			std::cout
				<< "VulkanFramebufferManager::Create - "
				"failed to create framebuffer at index "
				<< i
				<< '\n';

			return false;
		}
		/*
		* Move the successfully-created framebuffer
		* into the temporary vector.
		*/
		newFrameBuffer.push_back(std::move(buffer));
	}
	m_frameBuffers =std::move(newFrameBuffer);

	m_extents =extent;

	m_Initialize =true;


	return true;
}
void VulkanFramebufferManager::Destroy()
{
	/*
	 * Clearing the vector destroys every
	 * VulkanFramebuffer.
	 *
	 * VulkanFramebuffer::~VulkanFramebuffer()
	 * calls Destroy(), which destroys its VkFramebuffer.
	 */

	m_frameBuffers.clear();
	m_extents = {};
	m_Initialize = false;

}
const VulkanFramebuffer& VulkanFramebufferManager::Get(uint32_t imageIndex)const
{
	return m_frameBuffers.at(imageIndex);
}