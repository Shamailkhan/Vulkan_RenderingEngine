#include "VulkanUploadManager.h"

#include "VulkanSingleTimeCommand.h"
#include <iostream>


bool VulkanUploadManager::Initialize(const VulkanLogicalDeviceProvider& logicalDevice, const VulkanPhysicalDeviceProvider& physicalDevice,
	VulkanCommandPool& commandPool)
{
	if (logicalDevice.GetDevice() == VK_NULL_HANDLE || physicalDevice.getPhysicalDevice() || VK_NULL_HANDLE
		|| logicalDevice.GetGraphicQueue() == VK_NULL_HANDLE)
	{
		std::cerr << "VulkanUploadManager: Invalid providers.\n";
		return false;
	}
	
	if (!commandPool.InInitialized())
	{
		std::cerr << "VulkanUploadManager: Command pool not initialized.\n";
		return false;

	}


	if (commandPool.getDevice() != logicalDevice.GetDevice())
	{
		std::cerr << "VulkanUploadManager: Command pool belongs to "
			"a different logical device.\n";
		return false;
	}

	m_logicalDevice = &logicalDevice;
	m_physicalDevice = &physicalDevice;
	m_commandPool = &commandPool;
	isInitialize = true;

	return true;

}

bool VulkanUploadManager::UploadBuffer(const void* sourceData, VkDeviceSize size, VulkanBuffer& destinationBuffer, VkDeviceSize destinationOffset)
{
	if (!isInitialize || sourceData == nullptr || size == 0 || !destinationBuffer.isCreated())
		return false;

	if (!(destinationBuffer.GetUsage() & VK_BUFFER_USAGE_TRANSFER_DST_BIT))
	{
		std::cerr << "VulkanUploadManager: Destination buffer "
			"requires TRANSFER_DST usage.\n";
		return false;
	}
	if (destinationOffset > destinationBuffer.GetSize() ||
		size > destinationBuffer.GetSize() - destinationOffset)
	{
		std::cerr << "VulkanUploadManager: Upload exceeds buffer size.\n";
		return false;
	}

	return true;


}

const bool VulkanUploadManager::ValidateUpload(const void* sourceData, VkDeviceSize size, const VulkanBuffer& destinationBuffer, VkDeviceSize destinationOffset) const
{
	if (!ValidateUpload(
		sourceData,
		size,
		destinationBuffer,
		destinationOffset))
	{
		std::cerr << "VulkanUploadManager: Invalid upload request.\n";
		return false;
	}

	VulkanBufferDescription stagingDescrption{};

	stagingDescrption.size = size;
	stagingDescrption.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	stagingDescrption.memoryProperties = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT |
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

	
	VulkanBuffer stagingBuffer;

	if (!stagingBuffer.Create(
		*m_logicalDevice,
		*m_physicalDevice,
		stagingDescrption))
	{
		return false;
	}

	/*
	 * 2. Write CPU data to staging memory.
	 */
	if (!stagingBuffer.Write(sourceData, size))
	{
		return false;
	}
	/*
	* 3. Begin single-time command.
	*/

	VulkanSingleTimeCommand sigleTimeCommand;

	if (!sigleTimeCommand.Begin(m_logicalDevice->GetDevice(), *m_commandPool))
	{
		return false;
	}

	VkCommandBuffer comamdBuffer = sigleTimeCommand.GetCommandBuffer().getHandle();
	/*
	* 4. Record buffer copy.
	*/

	VkBufferCopy  copyRegion{};
	copyRegion.srcOffset = 0;
	copyRegion.dstOffset = destinationOffset;
	copyRegion.size = size;

	vkCmdCopyBuffer(
		comamdBuffer,
		stagingBuffer.getHandler(),
		destinationBuffer.getHandler(),
		1,
		&copyRegion
	);
	/*
		* 5. Submit using the graphics queue from our provider.
		*/
	if (!sigleTimeCommand.EndAndSubmit(
		m_logicalDevice->GetGraphicQueue()))
	{
		return false;
	}

	return true;

}
