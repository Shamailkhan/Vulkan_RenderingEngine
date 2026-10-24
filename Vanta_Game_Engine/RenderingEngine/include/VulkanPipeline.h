#pragma once
#include <vulkan/vulkan.h>
#include<string>
#include<vector>
#include "VulkanPipelineCache.h"
struct VulkanPipelineShaderStage
{
	VkShaderModule module = VK_NULL_HANDLE;
	VkShaderStageFlagBits Stage = VK_SHADER_STAGE_VERTEX_BIT;
	std::string entryPoint = "main";

};

struct VulkanPipelineVertexInput
{
	std::vector<VkVertexInputBindingDescription> binding;
	std::vector<VkVertexInputAttributeDescription> attribute;

};

struct VulkanPipelineViewportState
{
	uint32_t viewportCount = 1;
	uint32_t scissorCount = 1;
};

struct VulkanPipelineRasterizationSate
{	
	VkBool32 depthClampEnable = VK_FALSE;
	VkBool32 rasterizerDiscardEnable = VK_FALSE;
	VkPolygonMode polygonMode = VK_POLYGON_MODE_FILL;

	VkCullModeFlags CullMode = VK_CULL_MODE_BACK_BIT;
	VkFrontFace  frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

	VkBool32 depthBiasEnable = VK_FALSE;
	float depthBitConstantFactor = 0.0f;
	float depthBiasClamp = 0.0f;

	float depthBiasSlopFactor = 0.0f;
	float lineWidth = 1.0f;
};

struct VulkanPipelineMultiSampleState
{
	VkSampleCountFlagBits rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	VkBool32 sampleShadingEnable = VK_FALSE;
	float minSampleShading = 1.0f;
	std::vector<VkSampleMask> sampleMasks;

};

struct VulkanPipelineDepthStencilState
{
	VkBool32 depthTestEnable = VK_TRUE;
	VkBool32 depthWriteEnable = VK_TRUE;

	VkCompareOp depthCompareOp = VK_COMPARE_OP_LESS;

	VkBool32 depthBoundTestEnable = VK_FALSE;
	VkBool32 StencilTestEnable = VK_FALSE;
	VkStencilOpState front{};

	VkStencilOpState back{};
	float minDepthBound = 0.0f;
	float maxDepthBound = 1.0f;

};

struct VulkanPipelineColorBlenAttachment
{
	
	VkBool32 blendEnable = VK_FALSE;

	VkBlendFactor srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
	VkBlendFactor dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;

	VkBlendOp ColorBlendOp = VK_BLEND_OP_ADD;

	VkBlendFactor srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;

	VkBlendFactor dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	VkBlendOp aplhaBlendOp = VK_BLEND_OP_ADD;

	VkColorComponentFlags colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT |
		VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT |
		VK_COLOR_COMPONENT_A_BIT;

};

struct VulkanPipelineColorBlendState
{
	VkBool32 logicOpEnable = VK_FALSE;
	VkLogicOp logicOp = VK_LOGIC_OP_COPY;

	std::vector<VulkanPipelineColorBlenAttachment> attachments;
	float blendConstants[4] = { 0.0f,0.0f,0.0f,0.0f };


};
struct VulkanPipelineDynamicState
{
	std::vector<VkDynamicState> states = {
		   VK_DYNAMIC_STATE_VIEWPORT,
		   VK_DYNAMIC_STATE_SCISSOR
	};
};

struct VulkanGraphicPipelineCreateInfo
{
	std::vector<VulkanPipelineShaderStage> shader_Stage;

	VulkanPipelineVertexInput vertexInput;
	VkPrimitiveTopology Topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

	VkBool32 primitiveRestartEnable = VK_FALSE;

	VulkanPipelineViewportState viewportState;
	VulkanPipelineRasterizationSate rasterizationState;

	VulkanPipelineMultiSampleState multiSampling;
	VulkanPipelineDepthStencilState depthStencil;
	VulkanPipelineColorBlendState colorBlend;
	VulkanPipelineDynamicState dynamic_state;

	VkPipelineLayout pipeline_layput = VK_NULL_HANDLE;
	VkRenderPass RenderPass = VK_NULL_HANDLE;
	uint32_t subpass = 0;

};
class VulkanPipeline
{
public:
	VulkanPipeline() = default;
	~VulkanPipeline();

	VulkanPipeline(const VulkanPipeline&) = delete;
	VulkanPipeline& operator=(const VulkanPipeline&) = delete;

	VulkanPipeline(VulkanPipeline&& other) noexcept;
	VulkanPipeline& operator=(VulkanPipeline&& other) noexcept;

	bool Initialize(VkDevice device,VulkanPipelineCache& pipelineCache, const VulkanGraphicPipelineCreateInfo& createInfo);

	void Destroy();
	
	VkPipeline getHandle() { return m_pipeline; }
	bool IsInitialzied()const { return m_Initialized; }

private:
	bool CreateGraphicPipeline(VulkanPipelineCache& pipelineCache, const VulkanGraphicPipelineCreateInfo& creatInfo);

private:
	VkDevice m_device = VK_NULL_HANDLE;


	VkPipeline m_pipeline = VK_NULL_HANDLE;
	bool m_Initialized = false;
};

