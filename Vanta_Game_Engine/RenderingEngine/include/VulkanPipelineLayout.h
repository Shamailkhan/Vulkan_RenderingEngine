#pragma once
#include <vulkan/vulkan.h>
#include <vector>

class VulkanPipelineLayout
{
	VulkanPipelineLayout() = default;
	~VulkanPipelineLayout();

	VulkanPipelineLayout(const VulkanPipelineLayout&) = delete;
	VulkanPipelineLayout& operator=(const VulkanPipelineLayout&) = delete;

	VulkanPipelineLayout(VulkanPipelineLayout&& other) noexcept;
	VulkanPipelineLayout& operator=(VulkanPipelineLayout&& other) noexcept;


	bool Initialize(VkDevice Device, const std::vector<VkDescriptorSetLayout>& descriptionLayour,
		const std::vector<VkPushConstantRange>& pushConstantRanges = {});

	void Destroy();

	inline VkPipelineLayout getInstance()const { return m_pipelineLayout; }

	inline bool isInitialized() const { return m_initialized; }

private :
	bool CreatePipelineLayout(const std::vector<VkDescriptorSetLayout>& descriptorLaouts,
		const std::vector<VkPushConstantRange>& pushConstantRanges);
private:

	bool m_initialized = false;
	VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
	VkDevice m_Device = VK_NULL_HANDLE;

};

