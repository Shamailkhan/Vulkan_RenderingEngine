#include "VulkanCommandPool.h"
#include <iostream>
VulkanCommandPool::~VulkanCommandPool()
{
	Destroy();
}

VulkanCommandPool::VulkanCommandPool( VulkanCommandPool&& other) noexcept
{

    m_device = other.m_device;
    m_CommandPool = other.m_CommandPool;
    m_queueFamilyIndex = other.m_queueFamilyIndex;
    m_Initialize = other.m_Initialize;

    other.m_device = VK_NULL_HANDLE;
    other.m_CommandPool = VK_NULL_HANDLE;
    other.m_queueFamilyIndex = 0;
    other.m_Initialize = false;

}

VulkanCommandPool& VulkanCommandPool::operator=( VulkanCommandPool&& other) noexcept
{
    if (this != &other)
    {
        Destroy();

        m_device = other.m_device;
        m_CommandPool = other.m_CommandPool;
        m_queueFamilyIndex = other.m_queueFamilyIndex;
        m_Initialize = other.m_Initialize;

        other.m_device = VK_NULL_HANDLE;
        other.m_CommandPool = VK_NULL_HANDLE;
        other.m_queueFamilyIndex = 0;
        other.m_Initialize = false;
    }

    return *this;
}

bool VulkanCommandPool::Initialize(VkDevice device, uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags)
{
    if (m_Initialize)
        return true;

    if (device == VK_NULL_HANDLE)
    {
        std::cout << "VulkanCommandPool: Invalid VkDevice.\n";
        return false;
    }
    m_device = device;
    m_queueFamilyIndex = queueFamilyIndex;

    if (!CreateCommandPool(queueFamilyIndex, flags))
    {
        m_device = VK_NULL_HANDLE;
        m_queueFamilyIndex = 0;

        return false;


    }
    m_Initialize = true;
    return true;

}


void VulkanCommandPool::Destroy()
{

    if (m_device != VK_NULL_HANDLE || m_CommandPool != VK_NULL_HANDLE)
    {
        vkDestroyCommandPool(m_device, m_CommandPool, nullptr);
       
    }
    m_CommandPool = VK_NULL_HANDLE;
    m_device = VK_NULL_HANDLE;
    m_queueFamilyIndex = 0;
    m_Initialize = false;
}

bool VulkanCommandPool::CreateCommandPool(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags)
{
    VkCommandPoolCreateInfo CommandCreateInfo{};

    CommandCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    CommandCreateInfo.pNext = nullptr;
    CommandCreateInfo.queueFamilyIndex = queueFamilyIndex;
    CommandCreateInfo.flags = flags;

    VkResult result = vkCreateCommandPool(m_device, &CommandCreateInfo, nullptr,&m_CommandPool);
    if(result != VK_SUCCESS)
    {
        std::cerr
            << "VulkanCommandPool: Failed to create command pool. "
            << "VkResult = "
            << result
            << '\n';

        m_CommandPool = VK_NULL_HANDLE;

        return false;
    }
    return true;
}


