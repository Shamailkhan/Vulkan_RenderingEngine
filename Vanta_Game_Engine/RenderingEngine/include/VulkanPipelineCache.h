#pragma once
#include<vulkan/vulkan.h>
#include<string>
#include<vector>

class VulkanPipelineCache
{


	VulkanPipelineCache() = default;
	~VulkanPipelineCache();

	VulkanPipelineCache(const VulkanPipelineCache&) = delete;
	VulkanPipelineCache& operator=(const VulkanPipelineCache&) = delete;

	VulkanPipelineCache(VulkanPipelineCache&& other) noexcept;
	VulkanPipelineCache& operator=( VulkanPipelineCache&& other) noexcept;

	//Initialization

	bool Initialization(VkDevice device, const void* initialData=nullptr,size_t initialDataSize=0);


	bool InitializeFromFile(VkDevice device, const std::string& fileName);

	bool SaveToFile(const std::string FileName)const;
	void Destroy();

	VkPipelineCache GetInstance() const;

	bool IsInitialized() const { return m_initialized; }

private:

	bool CreatePipelineCache(const void* initialData, size_t initialDataSize);

	bool GetCacheData(std::vector<uint8_t>& data)const;

private:

	VkDevice m_Device = VK_NULL_HANDLE;
	VkPipelineCache m_pipelineCache = VK_NULL_HANDLE;

	bool m_initialized = false;

};

