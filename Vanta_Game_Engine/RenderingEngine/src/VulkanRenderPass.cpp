#include "VulkanRenderPass.h"
#include <iostream>
#include <array>
VulkanRenderPass::~VulkanRenderPass()
{
	Destroy();
}

VulkanRenderPass::VulkanRenderPass(VulkanRenderPass&& other) noexcept
	:m_Device(other.m_Device),
	m_Initialized(other.m_Initialized),
	m_RenderPass(other.m_RenderPass)
{
	other.m_Device = VK_NULL_HANDLE;
	other.m_RenderPass = VK_NULL_HANDLE;
	other.m_Initialized = false;
}

VulkanRenderPass& VulkanRenderPass::operator=( VulkanRenderPass&& other) noexcept
{
	if (this == &other)
		return *this;

	Destroy();

	m_Device = other.m_Device;
	m_RenderPass = other.m_RenderPass;
	m_Initialized = other.m_Initialized;

	other.m_Device = VK_NULL_HANDLE;
	other.m_RenderPass = VK_NULL_HANDLE;
	other.m_Initialized = false;


	return *this;
}

bool VulkanRenderPass::Initialize(VkDevice Device, VkFormat colorFormat, VkFormat depthFormat)
{
	if (m_Initialized)
		return true;
	if (Device == VK_NULL_HANDLE)
	{
		std::cout
			<< "VulkanRenderPass::Initialize - "
			<< "invalid logical device.\n";

		return false;
	}
	
	if (colorFormat == VK_FORMAT_UNDEFINED)
	{
		std::cout
			<< "VulkanRenderPass::Initialize - "
			<< "invalid color format.\n";

		return false;
	}

	if (depthFormat == VK_FORMAT_UNDEFINED)
	{
		std::cout
			<< "VulkanRenderPass::Initialize - "
			<< "invalid depth format.\n";

		return false;
	}
	m_Device = Device;

	if (!CreateRenderPass(colorFormat, depthFormat))
	{
		std::cout
			<< "VulkanRenderPass::Initialize - "
			<< "failed to create render pass.\n";

		Destroy();

		return false;
	}
	m_Initialized = true;
	return true;
}

void VulkanRenderPass::Destroy()
{
	if (m_Device != VK_NULL_HANDLE &&
		m_RenderPass != VK_NULL_HANDLE)
	{
		vkDestroyRenderPass(
			m_Device,
			m_RenderPass,
			nullptr
		);
	}


	m_RenderPass = VK_NULL_HANDLE;

	m_Device = VK_NULL_HANDLE;

	m_Initialized = false;

}
bool VulkanRenderPass::IsInitialized() const
{
	return m_Initialized;
}

bool VulkanRenderPass::CreateRenderPass(VkFormat colorFormat, VkFormat depthFormat)
{
	
	VkAttachmentDescription colorAttachment = CreateColorAttachment(colorFormat);
	VkAttachmentDescription DepthAttachment = CreateDepthAttachment(depthFormat);

	std::array<VkAttachmentDescription, 2>
		attachments =
	{
		colorAttachment,
		DepthAttachment
	};
	//Attachment Refrence

	VkAttachmentReference colorAttachmentRefrence{};

	colorAttachmentRefrence.attachment = 0;
	colorAttachmentRefrence.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference DepthAttachmentRefrence{};
	DepthAttachmentRefrence.attachment = 1;
	DepthAttachmentRefrence.layout= VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	// Subpass
	VkSubpassDescription subpass = CreateSubPass(colorAttachmentRefrence, DepthAttachmentRefrence);

	// Subpass dependency
	VkSubpassDependency dependency = CreateSubPassDependency();

	//Create Renderpass Create info 
	VkRenderPassCreateInfo renderpassCreateInfo{};

	renderpassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderpassCreateInfo.attachmentCount= static_cast<uint32_t>(attachments.size());
	renderpassCreateInfo.dependencyCount = 1;
	renderpassCreateInfo.pAttachments = attachments.data();
	renderpassCreateInfo.subpassCount = 1;
	renderpassCreateInfo.pSubpasses = &subpass;
	renderpassCreateInfo.pDependencies =&dependency;


	VkResult result = vkCreateRenderPass(m_Device, &renderpassCreateInfo, nullptr, &m_RenderPass);
	if (result != VK_SUCCESS)
	{
		std::cerr
			<< "VulkanRenderPass::CreateRenderPass - "
			<< "vkCreateRenderPass failed. Error: "
			<< result
			<< '\n';

		m_RenderPass = VK_NULL_HANDLE;

		return false;
	}

	return true;

}

VkAttachmentDescription VulkanRenderPass::CreateColorAttachment(VkFormat colorFormat) const
{
	VkAttachmentDescription colorAttachments{};
	colorAttachments.format = colorFormat;
	colorAttachments.samples = VK_SAMPLE_COUNT_1_BIT;
	// Clear the color buffer at the beginning
   // of the render pass.

	colorAttachments.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	// We want to present the rendered image,
	// so preserve the result after rendering.

	colorAttachments.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachments.stencilLoadOp= VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachments.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachments.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;;
	colorAttachments.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	return colorAttachments;
}



VkAttachmentDescription VulkanRenderPass::CreateDepthAttachment(VkFormat depthFormat) const
{
	VkAttachmentDescription depthAttachment{};

	depthAttachment.format = depthFormat;
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	// Clear depth buffer at the beginning
   // of every render pass.
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	// We don't need to preserve the depth buffer
	// after the render pass.
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	// We only use stencil if the selected depth format
	// contains a stencil component.
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	// Previous depth contents are not needed.
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	// Layout required while using the depth attachment.
	depthAttachment.finalLayout =
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	return depthAttachment;

}

VkSubpassDescription VulkanRenderPass::CreateSubPass(VkAttachmentReference& colorAttachmentRefrence, VkAttachmentReference& depthRefrence) const
{
	VkSubpassDescription subpass{};
	
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRefrence;
	subpass.pDepthStencilAttachment = &depthRefrence;
	return subpass;
}

VkSubpassDependency VulkanRenderPass::CreateSubPassDependency() const
{
	VkSubpassDependency dependency{};
	// External operations before our subpass.
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	// Our first and only subpass.
	dependency.dstSubpass = 0;
	// Wait for previous color output operations.
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
		| VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT
		| VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
	dependency.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
		| VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
		| VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
	return dependency;


}
