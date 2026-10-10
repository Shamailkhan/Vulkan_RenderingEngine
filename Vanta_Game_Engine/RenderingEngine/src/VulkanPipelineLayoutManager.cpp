#include "VulkanPipelineLayoutManager.h"
#include <iostream>
VulkanPipelineLayoutManager::~VulkanPipelineLayoutManager()
{
	Destroy();
}

bool VulkanPipelineLayoutManager::Initialize(VkDevice device)
{
	if (m_initialize)
		return true;
	if (device == VK_NULL_HANDLE)
	{

		std::cout
			<< "VulkanPipelineLayoutManager::Initialize - "
			"invalid VkDevice\n";

		return false;
	}
	m_Device = device;
	m_initialize = true;
	return true;
}
void VulkanPipelineLayoutManager::Destroy()
{
	DestroyAll();
	m_Device = VK_NULL_HANDLE;
	m_initialize = false;
}

VulkanPipelineLayout* VulkanPipelineLayoutManager::Create(std::string name, const std::vector<VkDescriptorSetLayout>& descriptorLayout, const std::vector<VkPushConstantRange>& pushRanges)
{
	if (!m_initialize)
	{
		std::cout
			<< "VulkanPipelineLayoutManager::Create - "
			"manager is not initialized\n";

		return nullptr;
	}
	if (name.empty())
	{
		std::cout
			<< "VulkanPipelineLayoutManager::Create - "
			"empty layout name\n";

		return nullptr;
	}

	if (Exisit(name))
	{
		std::cout
			<< "VulkanPipelineLayoutManager::Create - "
			"layout already exists: "
			<< name
			<< '\n';

		return nullptr;
	}

	auto layout = std::make_unique<VulkanPipelineLayout>();
	

	if (!layout->Initialize(
		m_Device,
		descriptorLayout,
		pushRanges))
	{
		return nullptr;
	}
	VulkanPipelineLayout* result = layout.get();
	m_layouts.emplace(name,std::move(layout));

	return result;

}

VulkanPipelineLayout* VulkanPipelineLayoutManager::Get(const std::string& name)
{
	auto layout = m_layouts.find(name);
	if (layout == m_layouts.end())
	{
		return nullptr;
	}
	 return &layout->second;
}

const VulkanPipelineLayout* VulkanPipelineLayoutManager::Get(const std::string& name) const
{
	auto iterator =
		m_layouts.find(name);

	if (iterator == m_layouts.end())
	{
		return nullptr;
	}

	return &iterator->second;
}

bool VulkanPipelineLayoutManager::Exisit(const std::string& name) const
{
	return m_layouts.find(name)
		!= m_layouts.end();
}

bool VulkanPipelineLayoutManager::Remove(const std::string& name)
{
	auto layout = m_layouts.find(name);
	if (layout == m_layouts.end())
	{
		return false;
	}
	m_layouts.erase(layout);
	return true;
}

void VulkanPipelineLayoutManager::DestroyAll()
{
	m_layouts.clear();
}

