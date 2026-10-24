#include "VulkanFramebuffer.h"
#include<iostream>

VulkanFramebuffer::~VulkanFramebuffer()
{
    Destroy();
}
VulkanFramebuffer::VulkanFramebuffer(VulkanFramebuffer&& other) noexcept
{
    m_device = other.m_device;
    m_FrameBuffer = other.m_FrameBuffer;
    m_extent = other.m_extent;
    IsIntialize =
        other.IsIntialize;


    other.m_device =
        VK_NULL_HANDLE;

    other.m_FrameBuffer =
        VK_NULL_HANDLE;

    other.m_extent =
    {};

    other.IsIntialize = false;


}

VulkanFramebuffer& VulkanFramebuffer::operator=(
    VulkanFramebuffer&& other) noexcept
{

    if (this != &other)
    {
        Destroy();


        m_device =
            other.m_device;

        m_FrameBuffer =
            other.m_FrameBuffer;

        m_extent =
            other.m_extent;

        IsIntialize =
            other.IsIntialize ;


        other.m_device =
            VK_NULL_HANDLE;

        other.m_FrameBuffer =
            VK_NULL_HANDLE;

        other.m_extent =
        {};

        other.IsIntialize = false;
    }

    return *this;

}

bool VulkanFramebuffer::Initialized(VkDevice device, const VulkanRenderPass& renderpass, const VulkanImageView& SwapchainImageView, const VulkanImageView& depthImageView, VkExtent2D extent)
{
    if (IsIntialize)
        return true;
    if (device == VK_NULL_HANDLE)
    {
        std::cout
            << "VulkanFramebuffer::Initialize - "
            "invalid VkDevice\n";

        return false;
    }
    if (!renderpass.IsInitialized())
    {
        std::cout
            << "VulkanFramebuffer::Initialize - "
            "render pass is not initialized\n";

        return false;
    }
    if (!SwapchainImageView.IsCreated())
    {
        std::cout
            << "VulkanFramebuffer::Initialize - "
            "swap chain image view is not initialized\n";

        return false;
    }

    if (!depthImageView.IsCreated())
    {
        std::cout
            << "VulkanFramebuffer::Initialize - "
            "depth image view is not initialized\n";

        return false;
    }
    if (extent.width == 0 || extent.height == 0)
    {
        std::cout
            << "VulkanFramebuffer::Initialize - "
            "invalid framebuffer extent\n";

        return false;
    }
    m_device = device;
    m_extent = extent;
    if (!CreateFrameBuffer(renderpass, SwapchainImageView, depthImageView))
    {
        m_device =
            VK_NULL_HANDLE;

        m_extent =
        {};

        return false;
    }
    IsIntialize = true;

    return true;

}

bool VulkanFramebuffer::CreateFrameBuffer(const VulkanRenderPass& renderPass, const VulkanImageView& swapChainImage, const VulkanImageView& depthImageView)
{
    /*
      * The attachment order MUST match the attachment
      * order defined inside VulkanRenderPass.
      *
      * Our render pass currently has:
      *
      * attachment 0 = color
      * attachment 1 = depth
      */

    VkImageView attachments[2] = {
        swapChainImage.GetHandle(),
        depthImageView.GetHandle()
    };

    VkFramebufferCreateInfo CreateInfo{};

    CreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    CreateInfo.pNext = nullptr;
    CreateInfo.flags = 0;
    CreateInfo.renderPass = renderPass.GetHandle();
    CreateInfo.attachmentCount = 2;
    CreateInfo.pAttachments = attachments;
    CreateInfo.width = m_extent.width;
    CreateInfo.height = m_extent.height;
    CreateInfo.layers = 1;

    VkResult result = vkCreateFramebuffer(m_device,&CreateInfo,nullptr,&m_FrameBuffer);
    if (result != VK_SUCCESS)
    {
        std::cerr
            << "Failed to create Vulkan framebuffer. "
            << "VkResult: "
            << result
            << '\n';

        m_FrameBuffer = VK_NULL_HANDLE;

        return false;
    }

    return true;
}

VkFramebuffer VulkanFramebuffer::getHandle() const
{
    return m_FrameBuffer;
}

void  VulkanFramebuffer::Destroy()
{

    if (m_FrameBuffer != VK_NULL_HANDLE && m_device != VK_NULL_HANDLE)
    {
        vkDestroyFramebuffer(m_device, m_FrameBuffer, nullptr);
    }
    m_FrameBuffer = VK_NULL_HANDLE;
    m_device = VK_NULL_HANDLE;
    m_extent = {};
    IsIntialize = false;


}