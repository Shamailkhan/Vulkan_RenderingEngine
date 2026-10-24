#pragma once

#include"VulkanFramebuffer.h"
#include "VulkanRenderPass.h"
#include "VulkanImageView.h"

#include "vulkan/vulkan.h"

#include <cstdint>
#include <cstddef>
#include <vector>


class VulkanFramebufferManager
{

	VulkanFramebufferManager() = default;
	~VulkanFramebufferManager();;

    // Non-copyable
    VulkanFramebufferManager(
        const VulkanFramebufferManager&) = delete;

    VulkanFramebufferManager& operator=(
        const VulkanFramebufferManager&) = delete;


    bool CreateFrame(VkDevice device,
        const VulkanRenderPass& renderpass,
        const std::vector<VulkanImageView>& swapChainImageView,
        const VulkanImageView& depthView,
        VkExtent2D extent);

    void Destroy();

    const VulkanFramebuffer& Get(uint32_t imageIndex)const;
    size_t count() const { m_frameBuffers.size(); }
    VkExtent2D GetExtent()const { return m_extents; }
    bool IsInitialized() { return m_Initialize; }

private:
    std::vector<VulkanFramebuffer> m_frameBuffers;
    VkExtent2D m_extents;
    bool m_Initialize = false;
};

