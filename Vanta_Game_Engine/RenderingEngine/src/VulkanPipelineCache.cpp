#include "VulkanPipelineCache.h"
#include<iostream>
#include<fstream>
#include<utility>

VulkanPipelineCache::~VulkanPipelineCache()
{
	Destroy();
}
VulkanPipelineCache::VulkanPipelineCache(VulkanPipelineCache&& other) noexcept
{

	m_Device = other.m_Device;
	m_pipelineCache = other.m_pipelineCache;
	m_initialized = other.m_initialized;

	other.m_Device = VK_NULL_HANDLE;
	other.m_initialized = false;
	other.m_pipelineCache = VK_NULL_HANDLE;

}
VulkanPipelineCache& VulkanPipelineCache::operator=(VulkanPipelineCache&& other) noexcept
{

	if (this != &other)
	{
		Destroy();
		m_Device = other.m_Device;
		m_pipelineCache = other.m_pipelineCache;
		m_initialized = other.m_initialized;

		other.m_Device = VK_NULL_HANDLE;
		other.m_initialized = false;
		other.m_pipelineCache = VK_NULL_HANDLE;
	}
	return *this;
}

bool VulkanPipelineCache::Initialization(VkDevice device, const void* initialData, size_t initialDataSize)
{
	if (m_initialized)
		return true;
	if (device == VK_NULL_HANDLE)
	{
		std::cout
			<< "VulkanPipelineCache::Initialize - "
			"invalid VkDevice\n";

		return false;
	}

	if (initialDataSize > 0 && initialData == nullptr)
	{
		std::cout
			<< "VulkanPipelineCache::Initialize - "
			"initial data is null\n";

		return false;
	}

	m_Device = device;

	if (!CreatePipelineCache(initialData, initialDataSize))
	{
		m_Device = VK_NULL_HANDLE;

		return false;
	}

	m_initialized = true;
	return true;

}

bool VulkanPipelineCache::InitializeFromFile(VkDevice device, const std::string& fileName)
{
	std::ifstream file(fileName, std::ios::binary | std::ios::ate);
	if (!file.is_open())
	{
		// No cache is not a fatal error.
		// We can simply create an empty cache.
		return Initialization(device);
	}
	std::streamsize size = file.tellg();

	if (size <= 0)
	{
		return Initialization(device);

	}

	file.seekg(0, std::ios::beg);
	std::vector<uint8_t> data(static_cast<size_t>(size));

	if (file.read(reinterpret_cast<char*>(data.data()),
		size))
	{
		std::cout
			<< "Failed to read pipeline cache file: "
			<< fileName
			<< '\n';

		return Initialization(device);
	}

	return Initialization(device, data.data(), data.size());

}

bool VulkanPipelineCache::SaveToFile(const std::string FileName) const
{
	std::vector<uint8_t> data;

	if (!GetCacheData(data))
	{
		std::cout<<
			"Failed to retrieve pipeline cache data\n";
		return false;
	}

	if (data.empty())
	{
		return true;
	}

	std::ofstream file(FileName, std::ios::binary | std::ios::trunc);

	if (!file.is_open())
	{
		std::cout
			<< "Failed to open pipeline cache file for writing: "
			<< FileName
			<< '\n';

		return false;
	}

	file.write(reinterpret_cast<const char*> (data.data()),
		static_cast<std::streamsize>(data.size()));

	return file.good();
}

void VulkanPipelineCache::Destroy()
{
	if (m_pipelineCache != VK_NULL_HANDLE && m_Device != VK_NULL_HANDLE)
	{
		vkDestroyPipelineCache(m_Device, m_pipelineCache, nullptr);
	}
	m_pipelineCache = VK_NULL_HANDLE;
	m_Device = VK_NULL_HANDLE;
	m_initialized = false;
}

bool VulkanPipelineCache::CreatePipelineCache(const void* initialData, size_t initialDataSize)
{
	VkPipelineCacheCreateInfo  pipelineCacheCreateInfo{};
	pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
	pipelineCacheCreateInfo.pNext = nullptr;

	pipelineCacheCreateInfo.flags = 0;
	pipelineCacheCreateInfo.initialDataSize = initialDataSize;
	pipelineCacheCreateInfo.pInitialData = initialData;

	VkResult result = vkCreatePipelineCache(m_Device, &pipelineCacheCreateInfo, nullptr,& m_pipelineCache);
	if (result != VK_SUCCESS)
	{
		std::cout
			<< "Failed to create Vulkan pipeline cache. "
			<< "VkResult: "
			<< result
			<< '\n';
		m_pipelineCache = VK_NULL_HANDLE;
		return false;
	}
	return true;


}

bool VulkanPipelineCache::GetCacheData(std::vector<uint8_t>& data) const
{
	if (!m_initialized || m_pipelineCache == VK_NULL_HANDLE)
	{
		return false;
	}

	size_t dataSize = 0;

	VkResult result = vkGetPipelineCacheData(m_Device, m_pipelineCache, &dataSize, nullptr);

	if (result != VK_SUCCESS)
	{
		return false;
	}

	if (dataSize == 0)
	{
		return false;
	}

	data.resize(dataSize);
	result = vkGetPipelineCacheData(m_Device, m_pipelineCache, &dataSize, nullptr);

	if (result != VK_SUCCESS)
	{
		data.clear();

		return false;
	}	
	data.resize(dataSize);

	return true;

}




