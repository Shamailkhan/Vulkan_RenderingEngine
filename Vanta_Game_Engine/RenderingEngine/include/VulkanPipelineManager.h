#pragma once

#include "VulkanPipeline.h"
#include "VulkanPipelineCache.h"
#include <vulkan/vulkan.h>

#include <memory>  
#include <string>
#include <unordered_map>
class VulkanPipelineManager
{

public:
	VulkanPipelineManager() = default;
	~VulkanPipelineManager();

	VulkanPipelineManager(const VulkanPipelineManager&) = delete;
	VulkanPipelineManager& operator=(const VulkanPipelineManager&) = delete;

	bool Initialize(VkDevice device, VulkanPipelineCache* pipelineCache);
	void Destroy();

	VulkanPipeline* CreatePipeline(const std::string& name, const VulkanGraphicPipelineCreateInfo& createInfo);
	VulkanPipeline* Get(const std::string& name);

	const VulkanPipeline* Get(
		const std::string& name
	) const;


	bool Exists(
		const std::string& name
	) const;

	bool Remove(const std::string& name	);


	void DestroyAll();


	VkDevice GetDevice() const { return m_device; }

	VulkanPipelineCache* GetPipelineCache() const { return pipelineCache; }

	bool IsInitialized() const { return m_initialized; }

private:
	VkDevice m_device = VK_NULL_HANDLE;

	VulkanPipelineCache* pipelineCache = nullptr;
	std::unordered_map<std::string, std::unique_ptr<VulkanPipeline>> m_pipelines;


    bool m_initialized = false;
};

