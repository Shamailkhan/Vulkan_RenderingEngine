#include "VulkanCommandBufferManager.h"
#include <iostream>
VulkanCommandBufferManager::~VulkanCommandBufferManager()
{
	Destroy();

}

bool VulkanCommandBufferManager::Initialize(VkDevice device, const QueueFamilyIndices& queueFamilyIndices, uint32_t graphicCommandBufferCount, uint32_t transferCommandBufferCount)
{
	if (m_initialized)
	{
		Destroy();
	}
	if (device == VK_NULL_HANDLE)
	{
		std::cout
			<< "VulkanCommandBufferManager: "
			"Invalid VkDevice.\n";

		return false;
	}

	if (!queueFamilyIndices.graphicFamily.has_value())
	{
		std::cout
			<< "VulkanCommandBufferManager: "
			"Graphics queue family is missing.\n";

		return false;
	}
	if (graphicCommandBufferCount == 0)
	{
		std::cout
			<< "VulkanCommandBufferManager: "
			"Graphics command buffer count cannot be zero.\n";

		return false;
	}

	m_Device = device;
	if (!CreateGraphicCommandPool(queueFamilyIndices.graphicFamily.value()))
	{
		Destroy();
		return false;

	}

	if (!AllocateGraphicCommandBuffer(graphicCommandBufferCount))
	{
		Destroy();
		return false;
	}

	/*
	* Transfer pool is optional.
	*/

	if (transferCommandBufferCount > 0)
	{
		if (!queueFamilyIndices.transferFamily.has_value())
		{
			std::cerr
				<< "VulkanCommandBufferManager: "
				"Transfer command buffers requested, "
				"but no transfer queue family exists.\n";

			Destroy();
			return false;
		}

		if (!CreateTransferCommandPool(queueFamilyIndices.transferFamily.value()))
		{
			Destroy();
			return false;
		}
		if (!AllocateTransferCommandBuffer(transferCommandBufferCount))
		{
			Destroy();
			return false;
		}
	}
	m_initialized = true;
	return true;


}

void VulkanCommandBufferManager::Destroy()
{
	/*
	 * Explicitly free command buffers first.
	 */

	FreeGraphicsCommandBuffers();
	FreeTransferCommandBufffers();


	/*
	 * Destroy pools after their command buffers
	 * have been released.
	 */
	m_graphicCommandPool.Destroy();
	m_TransferCommandPool.Destroy();

	m_hasTranferPool = false;
	m_Device = VK_NULL_HANDLE;
	m_initialized = false;


}

bool VulkanCommandBufferManager::AllocateGraphicCommandBuffer(uint32_t count)
{

	if (!m_graphicCommandPool.InInitialized())
	{
		std::cout
			<< "VulkanCommandBufferManager: "
			"Graphics command pool is not initialized.\n";

		return false;
	}
	return AllocateCommandBuffer(m_graphicCommandPool, m_graphicsCommandBuffers, count);

}

bool VulkanCommandBufferManager::AllocateTransferCommandBuffer(uint32_t count)
{
	if (!m_TransferCommandPool.InInitialized())
	{
		std::cerr
			<< "VulkanCommandBufferManager: "
			"Transfer command pool is not initialized.\n";

		return false;
	}
	return AllocateCommandBuffer(m_TransferCommandPool, m_TransferCommandBuffers, count);
}

void VulkanCommandBufferManager::FreeGraphicsCommandBuffers()
{
	FreeCommandBuffer(m_graphicCommandPool, m_graphicsCommandBuffers);
}

void VulkanCommandBufferManager::FreeTransferCommandBufffers()
{
	FreeCommandBuffer(m_TransferCommandPool, m_TransferCommandBuffers);
}

VulkanCommandBuffer& VulkanCommandBufferManager::getGraphicCommandBuffer(uint32_t intex)
{
	return m_graphicsCommandBuffers.at(intex);
}

const VulkanCommandBuffer& VulkanCommandBufferManager::GetGraphicsCommandBuffer(uint32_t index) const
{
	return m_graphicsCommandBuffers.at(index);
}

VulkanCommandBuffer& VulkanCommandBufferManager::getTransferCommandBuffer(uint32_t intex)
{
	return m_TransferCommandBuffers.at(intex);
}

const VulkanCommandBuffer& VulkanCommandBufferManager::getTransferCommandBuffer(uint32_t intex) const
{
	return m_TransferCommandBuffers.at(intex);
}

const std::vector<VulkanCommandBuffer>& VulkanCommandBufferManager::GetGraphicsCommandBuffers() const
{
	return m_graphicsCommandBuffers;
}

const std::vector<VulkanCommandBuffer>& VulkanCommandBufferManager::GetTransferCommandBuffers() const
{
	return m_TransferCommandBuffers;
}

VulkanCommandPool& VulkanCommandBufferManager::GetGraphicCommandPool()
{
	 return m_graphicCommandPool;
}

const VulkanCommandPool& VulkanCommandBufferManager::GetGraphicCommandPool() const
{
	return m_graphicCommandPool;
}

VulkanCommandPool& VulkanCommandBufferManager::GetTransferCommandPool()
{
	return m_TransferCommandPool;
}

const VulkanCommandPool& VulkanCommandBufferManager::GetTransferCommandPool() const
{
	return m_TransferCommandPool;
}

uint32_t VulkanCommandBufferManager::GetGraphicCommandBufferCount() const
{
	return static_cast<uint32_t>(m_graphicsCommandBuffers.size());
}

uint32_t VulkanCommandBufferManager::GetTransferCommandBufferCount() const
{
	return static_cast<uint32_t>(m_TransferCommandBuffers.size());
}

bool VulkanCommandBufferManager::CreateGraphicCommandPool(uint32_t queueFamilyIndex)
{
	return m_graphicCommandPool.Initialize(m_Device, queueFamilyIndex,
		VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);


}

bool VulkanCommandBufferManager::CreateTransferCommandPool(uint32_t queueFamilyIndex)
{
	if (!m_TransferCommandPool.Initialize(
		m_Device,
		queueFamilyIndex,
		VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT))
	{
		return false;
	}

	m_hasTranferPool = true;

	return true;
}

bool VulkanCommandBufferManager::AllocateCommandBuffer(VulkanCommandPool& pool, std::vector<VulkanCommandBuffer>& buffers, uint32_t count)
{
	if (!pool.InInitialized())
	{
		return false;
	}
	if (count == 0)
	{
		return false;
	}

	/*
	 * If buffers already exist, free them first.
	 */

	if (!buffers.empty())
	{
		FreeCommandBuffer(pool, buffers);
	}

	std::vector<VkCommandBuffer> handles(count);

	VkCommandBufferAllocateInfo allocateInfo{};
	allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocateInfo.pNext = nullptr;
	allocateInfo.commandPool = pool.GetHandle();
	allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocateInfo.commandBufferCount = count;

	VkResult result = vkAllocateCommandBuffers(m_Device, &allocateInfo, handles.data());

	if (result != VK_SUCCESS)
	{
		std::cout
			<< "VulkanCommandBufferManager: "
			"Failed to allocate command buffers. "
			"VkResult = "
			<< result
			<< '\n';

		return false;
	}
	buffers.reserve(count);

	for (VkCommandBuffer handle : handles)
	{
		VulkanCommandBuffer commandBuffer;
		commandBuffer.SetHandle(handle, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		buffers.push_back(std::move(commandBuffer));

	}
	return true;


}

void VulkanCommandBufferManager::FreeCommandBuffer(VulkanCommandPool& pool, std::vector<VulkanCommandBuffer>& buffers)
{

	if (buffers.empty())
	{
		return;
	}
	if (pool.GetHandle() == VK_NULL_HANDLE || m_Device == VK_NULL_HANDLE)
	{
		/*
         * This should normally never happen because
         * the pool must outlive its command buffers.
         */
		buffers.clear();
		return;
	}

	std::vector<VkCommandBuffer> handles;


	handles.resize(buffers.size());
	for (const VulkanCommandBuffer& buffer : buffers)
	{
		if (buffer.getHandle() != VK_NULL_HANDLE)
		{
			handles.push_back(buffer.getHandle());
		}
	}

	if (!handles.empty())
	{
		vkFreeCommandBuffers(m_Device, pool.GetHandle(), static_cast<uint32_t>(handles.size()), handles.data());

	}
	for (VulkanCommandBuffer& buffer : buffers)
	{
		buffer.Clear();
	}
	buffers.clear();
}



