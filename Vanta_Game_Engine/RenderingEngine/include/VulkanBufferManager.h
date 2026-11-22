#pragma once

#include "VulkanBuffer.h"

#include <vulkan/vulkan.h>
#include <memory>
#include <string>
#include <unordered_map>

class VulkanPhysicalDeviceProvider;
class VulkanLogicalDeviceProvider;
class  VulkanBufferManager
{

    VulkanBufferManager() = default;
    ~VulkanBufferManager();

    VulkanBufferManager(const VulkanBufferManager&) = delete;
    VulkanBufferManager& operator=(const VulkanBufferManager&) = delete;

    bool Initialize(const VulkanLogicalDeviceProvider& logicalDevice
        , const VulkanPhysicalDeviceProvider& physicalDevice);

    void Destroy();

    VulkanBuffer* Create(const std::string& name, const VulkanBufferDescription& description);

    VulkanBuffer* get(const std::string& name);

    const VulkanBuffer* get(const std::string& name)const;

    bool Remove(const std::string& name);

    bool Exists(const std::string& name) const;

    void DestroyAll();

    size_t Count() const;

    bool IsInitialized() const;

private:
    const VulkanLogicalDeviceProvider* m_logicalDevice = nullptr;
    const VulkanPhysicalDeviceProvider* m_physicalDevice = nullptr;
    std::unordered_map< std::string, std::unique_ptr<VulkanBuffer>> m_buffers;

    bool m_initialized = false;



};


