#pragma once
#include <VulkanPipelineLayout.h>
#include <string>
#include<unordered_map>

class VulkanPipelineLayoutManager
{

    VulkanPipelineLayoutManager() = default;
    ~VulkanPipelineLayoutManager();


    VulkanPipelineLayoutManager(
        const VulkanPipelineLayoutManager&) = delete;

    VulkanPipelineLayoutManager& operator=(
        const VulkanPipelineLayoutManager&) = delete;


    bool Initialize(VkDevice device);
    void Destroy();

    VulkanPipelineLayout* Create(std::string name, const std::vector<VkDescriptorSetLayout>& descriptorLayout = {},
        const std::vector<VkPushConstantRange>& pushRanges = {});
    VulkanPipelineLayout* Get(const std::string& name);

    const VulkanPipelineLayout* Get(
        const std::string& name
    ) const;


    bool Exisit(const std::string& name)const;

    bool Remove(const std::string& name);

    void DestroyAll();

   inline VkDevice GetDevice()const { return m_Device; }

   inline  bool isInitialized()const { return m_initialize; }

private:
    VkDevice m_Device = VK_NULL_HANDLE;

    std::unordered_map<std::string, VulkanPipelineLayout>m_layouts;

    bool m_initialize=false;

};

