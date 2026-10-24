#pragma once

#include <VulkanImageView.h>
#include <VulkanRenderPass.h>
#include <vulkan/vulkan.h>

#include <cstdint>
class VulkanFramebuffer
{
public:
	VulkanFramebuffer() = default;
	~VulkanFramebuffer();

    VulkanFramebuffer(const VulkanFramebuffer&) = delete;

    VulkanFramebuffer& operator=(
        const VulkanFramebuffer&) = delete;


    VulkanFramebuffer(VulkanFramebuffer&& other) noexcept;

    VulkanFramebuffer& operator=(
        VulkanFramebuffer&& other) noexcept;

    bool Initialized(VkDevice device,
        const VulkanRenderPass& renderpass,
        const VulkanImageView& SwapchainImageView, const VulkanImageView& depthImageView, VkExtent2D extent);
    void Destroy();

    VkFramebuffer getHandle()const { return m_FrameBuffer; }
    VkExtent2D GetExtend() const { return m_extent; }
    bool IsInitialized() const { return IsIntialize; }
private:
    bool CreateFrameBuffer(const VulkanRenderPass& renderPass, const VulkanImageView& swapChainImage,
        const VulkanImageView& depthImageView);
private:
    VkDevice m_device;
    VkFramebuffer m_FrameBuffer = VK_NULL_HANDLE;
    VkExtent2D m_extent{};
    bool IsIntialize = false;



};

