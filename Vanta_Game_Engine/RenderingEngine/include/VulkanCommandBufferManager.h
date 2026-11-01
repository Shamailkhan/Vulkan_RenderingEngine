#pragma once
#include "VulkanCommandBuffer.h"
#include "VulkanCommandPool.h"
#include "VulkanPhysicalDeviceProvider.h"

#include "vulkan/vulkan.h"

#include <cstdint>
#include <vector>
class   VulkanCommandBufferManager
{
public:
	VulkanCommandBufferManager() = default;
	~VulkanCommandBufferManager();

	VulkanCommandBufferManager& operator=(const VulkanCommandBufferManager&) = delete;
	VulkanCommandBufferManager(const VulkanCommandBufferManager&) = delete;

	bool Initialize(VkDevice device, const QueueFamilyIndices& queueFamilyIndices,
		uint32_t graphicCommandBufferCount, uint32_t transferCommandBufferCount = 0);
	void Destroy();
	bool AllocateGraphicCommandBuffer(uint32_t count);
	bool AllocateTransferCommandBuffer(uint32_t count);
	
	void FreeGraphicsCommandBuffers();
	void FreeTransferCommandBufffers();
	
	VulkanCommandBuffer& getGraphicCommandBuffer(uint32_t intex);
	const VulkanCommandBuffer& GetGraphicsCommandBuffer(
		uint32_t index
	) const;
	VulkanCommandBuffer& getTransferCommandBuffer(uint32_t intex);
	const VulkanCommandBuffer& getTransferCommandBuffer(uint32_t intex)const;

	const std::vector<VulkanCommandBuffer>& GetGraphicsCommandBuffers()const;
	const std::vector<VulkanCommandBuffer>& GetTransferCommandBuffers()const;

	VulkanCommandPool& GetGraphicCommandPool();
	const VulkanCommandPool& GetGraphicCommandPool()const;


	VulkanCommandPool& GetTransferCommandPool();
	const VulkanCommandPool& GetTransferCommandPool()const;

	uint32_t GetGraphicCommandBufferCount()const;
	uint32_t GetTransferCommandBufferCount()const;


	bool HasTransferPool()const { return m_hasTranferPool; }
	bool IsInitialized()const { return m_initialized; }

private:
	bool CreateGraphicCommandPool(uint32_t queueFamilyIndex);
	bool CreateTransferCommandPool(uint32_t queueFamilyIndex);
	
	bool AllocateCommandBuffer(VulkanCommandPool& pool, std::vector<VulkanCommandBuffer>& buffers, uint32_t count);

	void FreeCommandBuffer(VulkanCommandPool& pool, std::vector<VulkanCommandBuffer>& buffers);
private:

	VkDevice m_Device=VK_NULL_HANDLE;
	VulkanCommandPool m_graphicCommandPool;
	VulkanCommandPool m_TransferCommandPool;


	std::vector<VulkanCommandBuffer> m_graphicsCommandBuffers;
	std::vector<VulkanCommandBuffer> m_TransferCommandBuffers;

	bool m_hasTranferPool = false;
	bool m_initialized = false;

};

