#pragma once
#include <vulkan/vulkan.h>
#include<string>
#include<vector>

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

	VkCullModeFlags CULLmODE = VK_CULL_MODE_BACK_BIT;
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


class VulkanPipeline
{



};

