#include "VulkanSingleTimeCommand.h"
#include <iostream>
bool VulkanSingleTimeCommand::Begin(VkDevice device, VulkanCommandPool& commandPool)
{
    if (isActive)
    {
        std::cout
            << "VulkanSingleTimeCommand: "
            "Command is already active.\n";

        return false;

    }
    if (device == VK_NULL_HANDLE)
    {
        std::cout
            << "VulkanSingleTimeCommand: "
            "Invalid VkDevice.\n";

        return false;
    }


    if (!commandPool.InInitialized())
    {
        std::cout
            << "VulkanSingleTimeCommand: "
            "Command pool is not initialized.\n";

        return false;

    }
    m_device = device;
    m_commandPool = &commandPool;
    VkCommandBufferAllocateInfo AllocateInfo{};
    AllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    AllocateInfo.commandPool = commandPool.GetHandle();
    AllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    AllocateInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
    VkResult result = vkAllocateCommandBuffers(m_device, &AllocateInfo, &commandBuffer);
    if (result != VK_SUCCESS)
    {
        std::cerr
            << "VulkanSingleTimeCommand: "
            "Failed to allocate command buffer. "
            "VkResult = "
            << result
            << '\n';

        return false;
    }
    
    m_Commandbuffer.SetHandle(commandBuffer, VK_COMMAND_BUFFER_LEVEL_PRIMARY);


    if (!m_Commandbuffer.Begin(
        VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT))
    {
        vkFreeCommandBuffers(
            device,
            commandPool.GetHandle(),
            1,
            &commandBuffer
        );

        m_Commandbuffer.Clear();
        m_commandPool = nullptr;
        m_device = VK_NULL_HANDLE;

        return false;
    }


    isActive = true;
    return true;
}


bool VulkanSingleTimeCommand::EndAndSubmit(VkQueue queue)
{
    if (!isActive)
    {
        std::cerr
            << "VulkanSingleTimeCommand: "
            "No active command buffer.\n";

        return false;
    }
    if (queue == VK_NULL_HANDLE)
    {
        std::cout
            << "VulkanSingleTimeCommand: "
            "Invalid VkQueue.\n";

        return false;
    }


    if (!m_Commandbuffer.End())
    {
        return false;
    }
    VkCommandBuffer commandBuffer = m_Commandbuffer.getHandle();
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = nullptr;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    VkResult result = vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);

    if (result != VK_SUCCESS)
    {
        std::cerr
            << "VulkanSingleTimeCommand: "
            "Failed to submit command buffer. "
            "VkResult = "
            << result
            << '\n';

        return false;
    }
    result = vkQueueWaitIdle(queue);
    if (result != VK_SUCCESS)
    {
        std::cerr
            << "VulkanSingleTimeCommand: "
            "Failed waiting for queue. "
            "VkResult = "
            << result
            << '\n';

        return false;
    }
    vkFreeCommandBuffers(m_device, m_commandPool->GetHandle(), 1, &commandBuffer
    );
    m_Commandbuffer.Clear();
    m_commandPool = nullptr;
    m_device = VK_NULL_HANDLE;
    isActive = false;

    return true;


}

VulkanCommandBuffer& VulkanSingleTimeCommand::GetCommandBuffer()
{
    return m_Commandbuffer;
}

const VulkanCommandBuffer& VulkanSingleTimeCommand::GetCommandBuffer() const
{
    return m_Commandbuffer;
}

