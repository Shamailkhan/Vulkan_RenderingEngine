#include "VulkanPipelineManager.h"
#include <iostream>

VulkanPipelineManager::~VulkanPipelineManager()
{
	Destroy();

}

bool VulkanPipelineManager::Initialize(VkDevice device, VulkanPipelineCache* pipelineCache)
{

	if (m_initialized)
		return true;
	if (device == VK_NULL_HANDLE)
	{
		std::cout
			<< "VulkanPipelineManager::Initialize - "
			"invalid VkDevice\n";

		return false;
	}
	if (pipelineCache == nullptr)
	{
		std::cerr
			<< "VulkanPipelineManager::Initialize - "
			"pipeline cache is null\n";

		return false;
	}


	if (!pipelineCache->IsInitialized())
	{
		std::cerr
			<< "VulkanPipelineManager::Initialize - "
			"pipeline cache is not initialized\n";

		return false;

	}
	m_device = device;
	pipelineCache = pipelineCache;

	m_initialized = true;

	return true;







}
void VulkanPipelineManager::Destroy()
{
	DestroyAll();

	pipelineCache = nullptr;

	m_device = VK_NULL_HANDLE;

	m_initialized = false;
}

VulkanPipeline* VulkanPipelineManager::CreatePipeline(const std::string& name, const VulkanGraphicPipelineCreateInfo& createInfo)
{
	if (!m_initialized)
	{
		std::cout
			<< "VulkanPipelineManager::Create - "
			"manager is not initialized\n";

		return nullptr;
	}

	if (name.empty())
	{
		std::cout
			<< "VulkanPipelineManager::Create - "
			"empty pipeline name\n";

		return nullptr;
	}
	if (Exists(name))
	{
		std::cerr
			<< "VulkanPipelineManager::Create - "
			"pipeline already exists: "
			<< name
			<< '\n';

		return nullptr;
	}
	auto pipeline =
		std::make_unique<VulkanPipeline>();

	if (!pipeline->Initialize(m_device, *pipelineCache, createInfo))
	{
		return nullptr;
	}
	VulkanPipeline* result = pipeline.get();
	m_pipelines.emplace(name, std::move(pipeline));
	return result;
}
VulkanPipeline* VulkanPipelineManager::Get(const std::string& name)
{
	auto iterator =
		m_pipelines.find(name);


	if (iterator == m_pipelines.end())
	{
		return nullptr;
	}


	return iterator->second.get();

}

const VulkanPipeline* VulkanPipelineManager::Get(
	const std::string& name
) const
{
	auto iterator =
		m_pipelines.find(name);


	if (iterator == m_pipelines.end())
	{
		return nullptr;
	}


	return iterator->second.get();
}


bool VulkanPipelineManager::Exists(
	const std::string& name
) const
{
	return m_pipelines.find(name)
		!= m_pipelines.end();
}

bool VulkanPipelineManager::Remove(const std::string& name)
{
	auto iterator =
		m_pipelines.find(name);


	if (iterator == m_pipelines.end())
	{
		return false;
	}


	m_pipelines.erase(iterator);

	return true;
}


void VulkanPipelineManager::DestroyAll()
{
	m_pipelines.clear();
}


