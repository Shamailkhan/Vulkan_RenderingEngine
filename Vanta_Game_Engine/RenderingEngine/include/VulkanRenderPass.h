#pragma once
#include <vulkan/vulkan.h>
class VulkanRenderPass
{
public :

	VulkanRenderPass() = default;
	~VulkanRenderPass();
	//No copying 
	VulkanRenderPass(VulkanRenderPass&) = delete;
	VulkanRenderPass& operator=( VulkanRenderPass&) = delete;

	// Move support
	VulkanRenderPass( VulkanRenderPass&& other) noexcept;
	VulkanRenderPass& operator=(VulkanRenderPass&& other) noexcept;

	//Initialization
	bool Initialize(VkDevice Device, VkFormat colorFormat, VkFormat depthFormat);
	//Destruction

	void Destroy();
	bool IsInitialized() const;

	VkRenderPass GetHandle()const { return m_RenderPass; }
private:
	bool CreateRenderPass(VkFormat colorFormat, VkFormat depthFormat);
	VkAttachmentDescription CreateColorAttachment(VkFormat colorFormat)const;
	VkAttachmentDescription CreateDepthAttachment(VkFormat depthFormat)const;

	VkSubpassDescription CreateSubPass(VkAttachmentReference& colorAttachmentRefrence, 
		VkAttachmentReference& depthRefrence)const;
	// Subpass dependency

	VkSubpassDependency CreateSubPassDependency()const;
private:
	VkDevice m_Device = VK_NULL_HANDLE;
	VkRenderPass m_RenderPass = VK_NULL_HANDLE;

	bool m_Initialized = false;

};

