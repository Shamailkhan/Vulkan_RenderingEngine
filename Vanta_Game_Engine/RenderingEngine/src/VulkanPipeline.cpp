#include "VulkanPipeline.h"
#include <iostream>
#include <utility>


VulkanPipeline::~VulkanPipeline()
{
	Destroy();
}


VulkanPipeline::VulkanPipeline(VulkanPipeline&& other) noexcept
{
    m_device = other.m_device;
    m_pipeline = other.m_pipeline;
    m_Initialized = other.m_Initialized;

    other.m_device = VK_NULL_HANDLE;
    other.m_pipeline = VK_NULL_HANDLE;
    other.m_Initialized = false;
}
VulkanPipeline& VulkanPipeline::operator=(VulkanPipeline&& other) noexcept
{
    if (this != &other)
    {
        Destroy();

        m_device = other.m_device;
        m_pipeline = other.m_pipeline;
        m_Initialized = other.m_Initialized;

        other.m_device = VK_NULL_HANDLE;
        other.m_pipeline = VK_NULL_HANDLE;
        other.m_Initialized = false;
    }
    return *this;
}

bool VulkanPipeline::Initialize(VkDevice device, VulkanPipelineCache& pipeline_Cache, const VulkanGraphicPipelineCreateInfo& createInfo)
{
    if (m_Initialized)
        return true;

    if (device == VK_NULL_HANDLE)
    {
        std::cout
            << "VulkanPipeline::Initialize - "
            "invalid VkDevice\n";

        return false;
    }
    if (createInfo.shader_Stage.empty())
    {
        std::cout
            << "VulkanPipeline::Initialize - "
            "no shader stages supplied\n";

        return false;
    }
    if (createInfo.pipeline_layput == VK_NULL_HANDLE)
    {
        std::cerr
            << "VulkanPipeline::Initialize - "
            "invalid pipeline layout\n";

        return false;
    }
    if (createInfo.RenderPass == VK_NULL_HANDLE)
    {
        std::cout
            << "VulkanPipeline::Initialize - "
            "invalid render pass\n";

        return false;
    }
    for (const auto& shaderStage : createInfo.shader_Stage)
    {
        if (shaderStage.module == VK_NULL_HANDLE)
        {
            std::cerr
                << "VulkanPipeline::Initialize - "
                "invalid shader module\n";

            return false;
        }

        if (shaderStage.entryPoint.empty())
        {
            std::cerr
                << "VulkanPipeline::Initialize - "
                "empty shader entry point\n";

            return false;
        }
    }
    m_device = device;
    if (!CreateGraphicPipeline(pipeline_Cache, createInfo))
    {
        m_device = VK_NULL_HANDLE;

        return false;
    }

    m_Initialized = true;
    return true;
}



void VulkanPipeline::Destroy()
{
    if (m_pipeline != VK_NULL_HANDLE && m_device != VK_NULL_HANDLE)
    {

        vkDestroyPipeline(m_device, m_pipeline, nullptr);

    }
    m_pipeline = VK_NULL_HANDLE;
    m_device = VK_NULL_HANDLE;
    m_Initialized=false;
}

bool VulkanPipeline::CreateGraphicPipeline(VulkanPipelineCache& pipelineCache, const VulkanGraphicPipelineCreateInfo& creatInfo)
{
    ///....SHADER STAGES...................//////////////////

    std::vector<VkPipelineShaderStageCreateInfo> ShaderStages;

    ShaderStages.reserve(creatInfo.shader_Stage.size());
    for (const auto& shaderStage : creatInfo.shader_Stage)
    {

        VkPipelineShaderStageCreateInfo stageInfo{};

        stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        stageInfo.pNext = nullptr;
        stageInfo.flags = 0;
        stageInfo.stage = shaderStage.Stage;
        stageInfo.module = shaderStage.module;
        stageInfo.pName = shaderStage.entryPoint.c_str();
        stageInfo.pSpecializationInfo = nullptr;
        ShaderStages.push_back(stageInfo);
    }


    //////...................VERTEX INPUT........................//////
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};

    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.pNext = nullptr;
    vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(creatInfo.vertexInput.binding.size());

    vertexInputInfo.pVertexBindingDescriptions =
        creatInfo.vertexInput.binding.empty()
        ? nullptr
        : creatInfo.vertexInput.binding.data();

    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(creatInfo.vertexInput.attribute.size());
    vertexInputInfo.pVertexAttributeDescriptions = creatInfo.vertexInput.attribute.empty() ? nullptr : creatInfo.vertexInput.attribute.data();
      
    /////..................INPUT ASSEMBLY ................////

    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};

    inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyInfo.pNext = nullptr;
    inputAssemblyInfo.topology = creatInfo.Topology;
    inputAssemblyInfo.primitiveRestartEnable = creatInfo.primitiveRestartEnable;


    //////////////....................VIEWPORT............................//////////////

    VkPipelineViewportStateCreateInfo viewportInfo{};
    viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportInfo.pNext = nullptr;
    viewportInfo.viewportCount = creatInfo.viewportState.viewportCount;
    /*
     * If viewport/scissor are dynamic, Vulkan does not require
     * actual VkViewport/VkRect2D arrays here.
     */
    viewportInfo.pViewports = nullptr;
    viewportInfo.scissorCount = creatInfo.viewportState.scissorCount;



    /////////////.........Rasterization................./////////////////////

    VkPipelineRasterizationStateCreateInfo rasterizationCreateInfo{};
    rasterizationCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizationCreateInfo.pNext = nullptr;
    rasterizationCreateInfo.depthClampEnable = creatInfo.rasterizationState.depthClampEnable;
    rasterizationCreateInfo.rasterizerDiscardEnable = creatInfo.rasterizationState.rasterizerDiscardEnable;
    rasterizationCreateInfo.polygonMode = creatInfo.rasterizationState.polygonMode;
    rasterizationCreateInfo.cullMode = creatInfo.rasterizationState.CullMode;
    rasterizationCreateInfo.frontFace = creatInfo.rasterizationState.frontFace;
    rasterizationCreateInfo.depthBiasEnable = creatInfo.rasterizationState.depthBiasEnable;
    rasterizationCreateInfo.depthBiasConstantFactor = creatInfo.rasterizationState.depthBitConstantFactor;
    rasterizationCreateInfo.depthBiasClamp = creatInfo.rasterizationState.depthBiasClamp;
    rasterizationCreateInfo.depthBiasSlopeFactor = creatInfo.rasterizationState.depthBiasSlopFactor;
    rasterizationCreateInfo.lineWidth = creatInfo.rasterizationState.lineWidth;


    //////....MULTISAMPLING ................./////

    VkPipelineMultisampleStateCreateInfo multiSampleCreateInfo{};

    multiSampleCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multiSampleCreateInfo.pNext = nullptr;
    multiSampleCreateInfo.rasterizationSamples = creatInfo.multiSampling.rasterizationSamples;
    multiSampleCreateInfo.sampleShadingEnable = creatInfo.multiSampling.sampleShadingEnable;
    multiSampleCreateInfo.minSampleShading = creatInfo.multiSampling.minSampleShading;
    multiSampleCreateInfo.pSampleMask = creatInfo.multiSampling.sampleMasks.empty() ? nullptr : creatInfo.multiSampling.sampleMasks.data();
    multiSampleCreateInfo.alphaToCoverageEnable = VK_FALSE;
    multiSampleCreateInfo.alphaToOneEnable = VK_FALSE;


    /////............DEPTH /STENCIL..................////
    VkPipelineDepthStencilStateCreateInfo depthstencilCreateInfo{};
    depthstencilCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthstencilCreateInfo.pNext = nullptr;
    depthstencilCreateInfo.depthTestEnable = creatInfo.depthStencil.depthTestEnable;
    depthstencilCreateInfo.depthWriteEnable = creatInfo.depthStencil.depthWriteEnable;
    depthstencilCreateInfo.depthCompareOp = creatInfo.depthStencil.depthCompareOp;
    depthstencilCreateInfo.depthBoundsTestEnable = creatInfo.depthStencil.depthBoundTestEnable;
    depthstencilCreateInfo.stencilTestEnable = creatInfo.depthStencil.StencilTestEnable;
    depthstencilCreateInfo.front = creatInfo.depthStencil.front;
    depthstencilCreateInfo.back = creatInfo.depthStencil.back;
    depthstencilCreateInfo.minDepthBounds = creatInfo.depthStencil.minDepthBound;
    depthstencilCreateInfo.maxDepthBounds = creatInfo.depthStencil.maxDepthBound;





    //.................Color Blending................./////

    std::vector<VkPipelineColorBlendAttachmentState> blendAttachments;

    blendAttachments.reserve(creatInfo.colorBlend.attachments.size());

    for (const auto& attachments : creatInfo.colorBlend.attachments)
    {

        VkPipelineColorBlendAttachmentState blendAttachment{};

        blendAttachment.blendEnable = attachments.blendEnable;
        blendAttachment.srcColorBlendFactor = attachments.srcColorBlendFactor;
        blendAttachment.dstColorBlendFactor = attachments.dstColorBlendFactor;
        blendAttachment.srcAlphaBlendFactor = attachments.srcAlphaBlendFactor;
        blendAttachment.dstAlphaBlendFactor = attachments.dstAlphaBlendFactor;
        blendAttachment.alphaBlendOp = attachments.aplhaBlendOp;
        blendAttachment.colorBlendOp = attachments.ColorBlendOp;
        blendAttachment.colorWriteMask = attachments.colorWriteMask;
        blendAttachments.push_back(blendAttachment);

    }
    VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
    colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendInfo.pNext = nullptr;
    colorBlendInfo.logicOpEnable = creatInfo.colorBlend.logicOpEnable;
    colorBlendInfo.logicOp = creatInfo.colorBlend.logicOp;
    colorBlendInfo.attachmentCount = static_cast<uint32_t>(blendAttachments.size());
    colorBlendInfo.pAttachments = blendAttachments.empty() ? nullptr : blendAttachments.data();
    colorBlendInfo.blendConstants[0] = creatInfo.colorBlend.blendConstants[0];
    colorBlendInfo.blendConstants[1] = creatInfo.colorBlend.blendConstants[1];
    colorBlendInfo.blendConstants[2] = creatInfo.colorBlend.blendConstants[2];
    colorBlendInfo.blendConstants[3] = creatInfo.colorBlend.blendConstants[3];

    ////.............dynamic state......................///

    VkPipelineDynamicStateCreateInfo dynamicCreateInfo{};
    dynamicCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicCreateInfo.pNext = nullptr;
    dynamicCreateInfo.dynamicStateCount = static_cast<uint32_t>(creatInfo.dynamic_state.states.size());
    dynamicCreateInfo.pDynamicStates = creatInfo.dynamic_state.states.empty() ? nullptr : creatInfo.dynamic_state.states.data();

    //........GRAPHIC PIPELINE......................../////////

    VkGraphicsPipelineCreateInfo GraphicCreateInfo{};
    GraphicCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    GraphicCreateInfo.pNext = nullptr;
    GraphicCreateInfo.flags = 0;
    GraphicCreateInfo.stageCount = static_cast<uint32_t> (ShaderStages.size());
    GraphicCreateInfo.pStages = ShaderStages.data();
    GraphicCreateInfo.pVertexInputState = &vertexInputInfo;
    GraphicCreateInfo.pInputAssemblyState = &inputAssemblyInfo;
    GraphicCreateInfo.pTessellationState =nullptr;
    GraphicCreateInfo.pViewportState = &viewportInfo;
    GraphicCreateInfo.pRasterizationState = &rasterizationCreateInfo;
    GraphicCreateInfo.pMultisampleState = &multiSampleCreateInfo;
    GraphicCreateInfo.pDepthStencilState = &depthstencilCreateInfo;
    GraphicCreateInfo.pColorBlendState = &colorBlendInfo;
    GraphicCreateInfo.pDynamicState = creatInfo.dynamic_state.states.empty() ? nullptr : &dynamicCreateInfo;
    GraphicCreateInfo.layout = creatInfo.pipeline_layput;
    GraphicCreateInfo.renderPass = creatInfo.RenderPass;
    GraphicCreateInfo.subpass = creatInfo.subpass;
    GraphicCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
    GraphicCreateInfo.basePipelineIndex = -1;

    VkResult result = vkCreateGraphicsPipelines(m_device, pipelineCache.GetInstance(), 1, &GraphicCreateInfo, nullptr, &m_pipeline);

    if (result != VK_SUCCESS)
    {
        std::cout
            << "Failed to create Vulkan graphics pipeline. "
            << "VkResult: "
            << result
            << '\n';

        m_pipeline = VK_NULL_HANDLE;

        return false;
    }
    
    return true;
}
