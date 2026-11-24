#include "VulkanBufferManager.h"
#include "VulkanLogicalDeviceProvider.h"
#include "VulkanPhysicalDeviceProvider.h"

VulkanBufferManager::~VulkanBufferManager()
{
	Destroy();

}

bool VulkanBufferManager::Initialize(const VulkanLogicalDeviceProvider& logicalDevice, const VulkanPhysicalDeviceProvider& physicalDevice)
{
	if (m_initialized)
		Destroy();

	if (logicalDevice.GetDevice() == VK_NULL_HANDLE || physicalDevice.getPhysicalDevice() == nullptr)
	{
		std::cerr << "VulkanBufferManager: Invalid providers.\n";
		return false;
	}
	m_logicalDevice = &logicalDevice;
	m_physicalDevice = &physicalDevice;

	m_initialized = true;
	return true;

}

VulkanBuffer* VulkanBufferManager::Create(const std::string& name, const VulkanBufferDescription& description)
{
	if (!m_initialized)
	{
		std::cerr << "VulkanBufferManager: Not initialized.\n";
		return nullptr;
	}
	if (name.empty() || Exists(name))
	{
		std::cerr << "VulkanBufferManager: Invalid or duplicate name.\n";
		return nullptr;
	}

	auto buffer = std::make_unique<VulkanBuffer>(&m_logicalDevice, *m_physicalDevice, description);
	auto buffer = std::make_unique<VulkanBuffer>();

	if (!buffer->Create(
		*m_logicalDevice,
		*m_physicalDevice,
		description))
	{
		std::cerr << "VulkanBufferManager: Buffer creation failed: "
			<< name << '\n';
		return nullptr;
	}

	VulkanBuffer* result = buffer.get();

	m_buffers.emplace(name, std::move(buffer));

	return result;

}

VulkanBuffer* VulkanBufferManager::get(const std::string& name)
{
	auto it = m_buffers.find(name);

	return it != m_buffers.end() ? it->second.get() : nullptr;

}

bool VulkanBufferManager::Remove(
	const std::string& name
)
{
	return m_buffers.erase(name) > 0;
}

bool VulkanBufferManager::Exists(
	const std::string& name
) const
{
	return m_buffers.find(name) != m_buffers.end();
}

void VulkanBufferManager::DestroyAll()
{
	m_buffers.clear();
}

void VulkanBufferManager::Destroy()
{
	DestroyAll();

	m_logicalDevice = nullptr;
	m_physicalDevice = nullptr;

	m_initialized = false;
}

size_t VulkanBufferManager::Count() const
{
	return m_buffers.size();
}

bool VulkanBufferManager::IsInitialized() const
{

	return m_initialized;
}