#include "VulkanLogicalDeviceProvider.h"
#include <iostream>
#include <set>
namespace
{
    // ------------------------------------------------------------
    // Required device extensions
    // ------------------------------------------------------------

    const std::vector<const char*> REQUIRED_DEVICE_EXTENSIONS =
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };


    // ------------------------------------------------------------
    // Queue priority
    // ------------------------------------------------------------

    constexpr float DEFAULT_QUEUE_PRIORITY = 1.0f;
}


VulkanLogicalDeviceProvider::VulkanLogicalDeviceProvider(VkPhysicalDevice physicalDevice, const QueueFamilyIndices& queueIndices)
    :m_physicalDevice(physicalDevice),
    m_queueFamilyIndices(queueIndices)
{

}

VulkanLogicalDeviceProvider::~VulkanLogicalDeviceProvider()
{
    DestroyDevice();
}

bool VulkanLogicalDeviceProvider::Initiialize()
{
    if(m_initialized)
	return true;

    if (m_physicalDevice == VK_NULL_HANDLE)
    {
        std::cout << "Cannot create logical device: "
            << "physical device is null.\n";

        return false;

    }

    if (!m_queueFamilyIndices.isCompleted())
    {
        std::cerr
            << "Cannot create logical device: "
            << "required queue families are missing.\n";

        return false;
    }
    // Create logical device

    if (!CreateLogicalDevice())
    {
        return false;
    }
    std::cout
        << "Vulkan logical device initialized successfully."
        << std::endl;

    m_initialized = true;

    return true;


}

void VulkanLogicalDeviceProvider::DestroyDevice()
{
    if (m_device == VK_NULL_HANDLE)
    {
        m_initialized = false;
        return;
    }

    vkDeviceWaitIdle(m_device);
    vkDestroyDevice(m_device, nullptr);

    m_device = VK_NULL_HANDLE;

    m_graphicsQueue = VK_NULL_HANDLE;
    m_presentQueue = VK_NULL_HANDLE;
    m_computeQueue = VK_NULL_HANDLE;
    m_transferQueue = VK_NULL_HANDLE;

    m_initialized = false;


    std::cout
        << "Vulkan logical device destroyed."
        << std::endl;

}

VkDevice VulkanLogicalDeviceProvider::GetDevice() const
{
	return m_device;
}

VkQueue VulkanLogicalDeviceProvider::GetGraphicQueue() const
{
	return m_graphicsQueue;
}

VkQueue VulkanLogicalDeviceProvider::GetPresentQueue() const
{
	return m_presentQueue;
}

VkQueue VulkanLogicalDeviceProvider::GetTransferQueue() const
{
	return m_transferQueue;
}

VkQueue VulkanLogicalDeviceProvider::GetComputeQueue() const
{
	return m_computeQueue;
}
bool VulkanLogicalDeviceProvider::CreateLogicalDevice()
{
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfo = CreateQueueCreateInfo();


    if (queueCreateInfo.empty())
    {
        std::cout
            << "Failed to create queue create information.\n";

        return false;
    }

    VkPhysicalDeviceFeatures devicefeatiure{};
    const std::vector<const char*> requiredExtensions =
        GetRequiredDeviceExtensions();

    VkDeviceCreateInfo deviceCreateInfo{};
    
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext = nullptr;
    deviceCreateInfo.flags = 0;
    
    deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t> (queueCreateInfo.size());
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfo.data();

    // Device features
    deviceCreateInfo.pEnabledFeatures =&devicefeatiure;
    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t> (requiredExtensions.size());
    deviceCreateInfo.ppEnabledExtensionNames= requiredExtensions.data();

    deviceCreateInfo.enabledLayerCount = 0;
    deviceCreateInfo.ppEnabledLayerNames = nullptr;

    //craeteLogical device 

    VkResult result =
        vkCreateDevice(m_physicalDevice, &deviceCreateInfo, nullptr, &m_device);
    if (result != VK_SUCCESS)
    {
        std::cout
            << "Failed to create logical device. "
            << "VkResult = "
            << result
            << std::endl;

        m_device = VK_NULL_HANDLE;

        return false;
    }


    // Retrieve graphics queue

    vkGetDeviceQueue(m_device,m_queueFamilyIndices.graphicFamily.value(),0,&m_graphicsQueue);
    vkGetDeviceQueue(m_device,m_queueFamilyIndices.presentFamily.value(),0,&m_presentQueue);
   

    if (m_queueFamilyIndices.transferFamily.has_value())
    {
        vkGetDeviceQueue(m_device, m_queueFamilyIndices.transferFamily.value(), 0, &m_transferQueue);

    }
    if (m_queueFamilyIndices.computeFamily.has_value())
    {
        vkGetDeviceQueue(m_device, m_queueFamilyIndices.computeFamily.value(), 0, &m_computeQueue);
    }

    return true;
}

std::vector<VkDeviceQueueCreateInfo> VulkanLogicalDeviceProvider::CreateQueueCreateInfo() const
{
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfo_vector;

    std::set<uint32_t> uniqueFamilies;


    if (m_queueFamilyIndices.graphicFamily.has_value())
    {
        uniqueFamilies.insert(
            m_queueFamilyIndices.graphicFamily.value()
        );
    }
    // --------------------------------------------------------
        // Present queue
        // --------------------------------------------------------

        if (m_queueFamilyIndices.presentFamily.has_value())
        {
            uniqueFamilies.insert(
                m_queueFamilyIndices.presentFamily.value()
            );
        }


    // --------------------------------------------------------
    // Compute queue
    // --------------------------------------------------------

    if (m_queueFamilyIndices.computeFamily.has_value())
    {
        uniqueFamilies.insert(
            m_queueFamilyIndices.computeFamily.value()
        );
    }


    // --------------------------------------------------------
    // Transfer queue
    // --------------------------------------------------------

    if (m_queueFamilyIndices.transferFamily.has_value())
    {
        uniqueFamilies.insert(
            m_queueFamilyIndices.transferFamily.value()
        );
    }

    for (uint32_t queueFamily : uniqueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.pNext = nullptr;
        queueCreateInfo.queueFamilyIndex=queueFamily;
        queueCreateInfo.flags = 0;
        // We currently request one queue
        // from each queue family.
        queueCreateInfo.queueCount=1;
        queueCreateInfo.pQueuePriorities = &DEFAULT_QUEUE_PRIORITY;
        

        queueCreateInfo_vector.push_back(queueCreateInfo);


    }
    return queueCreateInfo_vector;
}
std::vector<const char*> VulkanLogicalDeviceProvider::GetRequiredDeviceExtensions() const
{
    return REQUIRED_DEVICE_EXTENSIONS;
}