//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_pipeline_state.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_context.hpp"
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_device.hpp"
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_shader.hpp"
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_render_pass.hpp"
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_swap_chain.hpp"
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_utilities.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    //--------------------------------------------------------------
    VulkanGraphicsPipelineState::VulkanGraphicsPipelineState(VulkanGraphicsDevice *device, const GraphicsPipelineStateDescription &description) : GraphicsPipelineState(device, description) {
        VkPipelineVertexInputStateCreateInfo pipeline_vertex_input_state_create_info = { };
        pipeline_vertex_input_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        pipeline_vertex_input_state_create_info.vertexBindingDescriptionCount = 0;
        pipeline_vertex_input_state_create_info.pVertexBindingDescriptions = nullptr;
        pipeline_vertex_input_state_create_info.vertexAttributeDescriptionCount = 0;
        pipeline_vertex_input_state_create_info.pVertexAttributeDescriptions = nullptr;

        VkPipelineInputAssemblyStateCreateInfo pipeline_input_assembly_state_create_info = { };
        pipeline_input_assembly_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        pipeline_input_assembly_state_create_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        pipeline_input_assembly_state_create_info.primitiveRestartEnable = VK_FALSE;

        VkExtent2D swap_chain_extent = device->GetContext()->GetSwapChain()->GetExtent();
        VkViewport viewport = { };
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float32>(swap_chain_extent.width);
        viewport.height = static_cast<float32>(swap_chain_extent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor = { };
        scissor.offset = { 0, 0 };
        scissor.extent = swap_chain_extent;

        VkPipelineViewportStateCreateInfo pipeline_viewport_state_create_info = { };
        pipeline_viewport_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        pipeline_viewport_state_create_info.viewportCount = 1;
        pipeline_viewport_state_create_info.pViewports = &viewport;
        pipeline_viewport_state_create_info.scissorCount = 1;
        pipeline_viewport_state_create_info.pScissors = &scissor;

        VkPipelineRasterizationStateCreateInfo pipeline_rasterization_state_create_info{};
        pipeline_rasterization_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        pipeline_rasterization_state_create_info.depthClampEnable = VK_FALSE;
        pipeline_rasterization_state_create_info.rasterizerDiscardEnable = VK_FALSE;
        pipeline_rasterization_state_create_info.polygonMode = VulkanGraphicsUtilities::GetPolygonMode(description.rasterizer_description.polygon_mode);
        pipeline_rasterization_state_create_info.lineWidth = 1.0f;
        pipeline_rasterization_state_create_info.cullMode = VulkanGraphicsUtilities::GetCullingMode(description.rasterizer_description.culling_mode);
        pipeline_rasterization_state_create_info.frontFace = VulkanGraphicsUtilities::GetCullingFrontFaceMode(description.rasterizer_description.culling_front_face_mode);
        pipeline_rasterization_state_create_info.depthBiasEnable = VK_FALSE;

        VkPipelineMultisampleStateCreateInfo pipeline_multisample_state_create_info = { };
        pipeline_multisample_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        pipeline_multisample_state_create_info.sampleShadingEnable = VK_FALSE;
        pipeline_multisample_state_create_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        pipeline_multisample_state_create_info.minSampleShading = 1.0f;
        pipeline_multisample_state_create_info.pSampleMask = nullptr;
        pipeline_multisample_state_create_info.alphaToCoverageEnable = VK_FALSE;
        pipeline_multisample_state_create_info.alphaToOneEnable = VK_FALSE;

        VkPipelineColorBlendAttachmentState pipeline_color_blend_attachment_state = { };
        pipeline_color_blend_attachment_state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        pipeline_color_blend_attachment_state.blendEnable = description.blending_description.blending_enabled;
        pipeline_color_blend_attachment_state.srcColorBlendFactor = VulkanGraphicsUtilities::GetBlendingFactor(description.blending_description.color_function.source_factor);
        pipeline_color_blend_attachment_state.dstColorBlendFactor = VulkanGraphicsUtilities::GetBlendingFactor(description.blending_description.color_function.destination_factor);
        pipeline_color_blend_attachment_state.colorBlendOp = VulkanGraphicsUtilities::GetBlendingOperator(description.blending_description.color_function.function_operator);
        pipeline_color_blend_attachment_state.srcAlphaBlendFactor = VulkanGraphicsUtilities::GetBlendingFactor(description.blending_description.alpha_function.source_factor);
        pipeline_color_blend_attachment_state.dstAlphaBlendFactor = VulkanGraphicsUtilities::GetBlendingFactor(description.blending_description.alpha_function.destination_factor);
        pipeline_color_blend_attachment_state.alphaBlendOp = VulkanGraphicsUtilities::GetBlendingOperator(description.blending_description.alpha_function.function_operator);

        VkPipelineColorBlendStateCreateInfo pipeline_color_blend_state_create_info = { };
        pipeline_color_blend_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        pipeline_color_blend_state_create_info.logicOpEnable = VK_FALSE;
        pipeline_color_blend_state_create_info.attachmentCount = 1;
        pipeline_color_blend_state_create_info.pAttachments = &pipeline_color_blend_attachment_state;

        VkDynamicState dynamic_states[] = {
            VK_DYNAMIC_STATE_VIEWPORT,
        };
        VkPipelineDynamicStateCreateInfo dynamicState = { };
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = 1;
        dynamicState.pDynamicStates = dynamic_states;

        VkPipelineLayoutCreateInfo pipeline_layout_create_info = { };
        pipeline_layout_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipeline_layout_create_info.setLayoutCount = 0;
        pipeline_layout_create_info.pSetLayouts = nullptr;
        pipeline_layout_create_info.pushConstantRangeCount = 0;
        pipeline_layout_create_info.pPushConstantRanges = nullptr;
        HYP_VULKAN_CHECK(vkCreatePipelineLayout(device->GetDevice(), &pipeline_layout_create_info, nullptr, &m_pipeline_layout));

        HYP_ASSERT(description.vertex_shader);
        HYP_ASSERT(description.vertex_shader->GetDescription().stage == GraphicsShaderStageFlags::Vertex);
        VkPipelineShaderStageCreateInfo vertex_shader_state_create_info = { };
        vertex_shader_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertex_shader_state_create_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertex_shader_state_create_info.module = static_cast<VulkanGraphicsShader *>(description.vertex_shader)->GetShaderModule();
        vertex_shader_state_create_info.pName = description.vertex_shader->GetDescription().entry_point;

        HYP_ASSERT(description.pixel_shader);
        HYP_ASSERT(description.pixel_shader->GetDescription().stage == GraphicsShaderStageFlags::Pixel);
        VkPipelineShaderStageCreateInfo pixel_shader_state_create_info = { };
        pixel_shader_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        pixel_shader_state_create_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        pixel_shader_state_create_info.module = static_cast<VulkanGraphicsShader *>(description.pixel_shader)->GetShaderModule();
        pixel_shader_state_create_info.pName = description.pixel_shader->GetDescription().entry_point;

        VkPipelineShaderStageCreateInfo shader_stages[] = { vertex_shader_state_create_info, pixel_shader_state_create_info };

        HYP_ASSERT(description.render_pass);
        VkGraphicsPipelineCreateInfo graphics_pipeline_create_info = { };
        graphics_pipeline_create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        graphics_pipeline_create_info.stageCount = 2;
        graphics_pipeline_create_info.pStages = shader_stages;
        graphics_pipeline_create_info.pVertexInputState = &pipeline_vertex_input_state_create_info;
        graphics_pipeline_create_info.pInputAssemblyState = &pipeline_input_assembly_state_create_info;
        graphics_pipeline_create_info.pViewportState = &pipeline_viewport_state_create_info;
        graphics_pipeline_create_info.pRasterizationState = &pipeline_rasterization_state_create_info;
        graphics_pipeline_create_info.pMultisampleState = &pipeline_multisample_state_create_info;
        graphics_pipeline_create_info.pDepthStencilState = nullptr;
        graphics_pipeline_create_info.pColorBlendState = &pipeline_color_blend_state_create_info;
        graphics_pipeline_create_info.pDynamicState = nullptr;
        graphics_pipeline_create_info.layout = m_pipeline_layout;
        graphics_pipeline_create_info.renderPass = reinterpret_cast<VulkanGraphicsRenderPass *>(description.render_pass)->GetRenderPass();
        graphics_pipeline_create_info.subpass = 0;
        graphics_pipeline_create_info.basePipelineHandle = VK_NULL_HANDLE;
        graphics_pipeline_create_info.basePipelineIndex = -1;

        vkCreateGraphicsPipelines(device->GetDevice(), VK_NULL_HANDLE, 1, &graphics_pipeline_create_info, nullptr, &m_graphics_pipeline);
    }

    //--------------------------------------------------------------
    VulkanGraphicsPipelineState::~VulkanGraphicsPipelineState() {
        VulkanGraphicsDevice *device = static_cast<VulkanGraphicsDevice *>(GetDevice());
        vkDestroyPipelineLayout(device->GetDevice(), m_pipeline_layout, nullptr);
        vkDestroyPipeline(device->GetDevice(), m_graphics_pipeline, nullptr);
    }

}