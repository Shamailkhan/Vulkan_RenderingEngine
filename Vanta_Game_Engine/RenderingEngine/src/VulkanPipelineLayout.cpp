#include "VulkanPipelineLayout.h"
#include <iostream>
VulkanPipelineLayout::~VulkanPipelineLayout()
{
	Destroy();
}
VulkanPipelineLayout::VulkanPipelineLayout(VulkanPipelineLayout&& other) noexcept
{
	m_Device = other.m_Device;
	m_pipelineLayout = other.m_pipelineLayout;
	m_initialized = other.m_initialized;
	other.m_Device = VK_NULL_HANDLE;
	other.m_pipelineLayout = VK_NULL_HANDLE;
	other.m_initialized = false;

}

VulkanPipelineLayout& VulkanPipelineLayout::operator=(VulkanPipelineLayout&& other) noexcept
{

	if (this != &other)
	{
		Destroy();

		m_Device = other.m_Device;
		m_pipelineLayout = other.m_pipelineLayout;
		m_initialized = other.m_initialized;

		other.m_Device = VK_NULL_HANDLE;
		other.m_pipelineLayout = VK_NULL_HANDLE;
		other.m_initialized = false;
	}

	return *this;


}

bool VulkanPipelineLayout::Initialize(VkDevice Device, const std::vector<VkDescriptorSetLayout>& descriptionLayour, 
	const std::vector<VkPushConstantRange>& pushConstantRanges)
{
	if (m_initialized)
		return true;

	if (Device == VK_NULL_HANDLE)
	{
		std::cout
			<< "VulkanPipelineLayout::Initialize - invalid VkDevice\n";

		return false;
	}
	m_Device = Device;
	if (!CreatePipelineLayout(descriptionLayour, pushConstantRanges))
	{
		m_Device = VK_NULL_HANDLE;
		return false;
	}
	m_initialized = true;
	return true;

}



void VulkanPipelineLayout::Destroy()
{
	if (m_pipelineLayout != VK_NULL_HANDLE && m_Device !=VK_NULL_HANDLE)
	{
		vkDestroyPipelineLayout(m_Device, m_pipelineLayout, nullptr);
	}

	m_Device = VK_NULL_HANDLE;
	m_pipelineLayout = VK_NULL_HANDLE;
	m_initialized = false;
}

bool VulkanPipelineLayout::CreatePipelineLayout(const std::vector<VkDescriptorSetLayout>& descriptorLaouts,
	const std::vector<VkPushConstantRange>& pushConstantRanges)
{

	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo;

	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCreateInfo.pNext = nullptr;
	pipelineLayoutCreateInfo.setLayoutCount = static_cast<uint32_t>(descriptorLaouts.size());
	pipelineLayoutCreateInfo.pSetLayouts = descriptorLaouts.empty() ? nullptr :descriptorLaouts.data();
	pipelineLayoutCreateInfo.pushConstantRangeCount= static_cast<uint32_t>(pushConstantRanges.size());
	pipelineLayoutCreateInfo.pPushConstantRanges= pushConstantRanges.empty() ? nullptr : pushConstantRanges.data();

	VkResult result = vkCreatePipelineLayout(m_Device, &pipelineLayoutCreateInfo, nullptr, &m_pipelineLayout);

	if (result != VK_SUCCESS)
	{
		std::cout
			<< "Failed to create Vulkan pipeline layout. "
			<< "VkResult: "
			<< result
			<< '\n';

		m_pipelineLayout = VK_NULL_HANDLE;

		return false;
	}
	return true;
}