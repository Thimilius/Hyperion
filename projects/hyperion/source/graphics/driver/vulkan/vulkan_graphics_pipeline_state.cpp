//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_pipeline_state.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_device.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    //--------------------------------------------------------------
    VulkanGraphicsPipelineState::VulkanGraphicsPipelineState(VulkanGraphicsDevice *device, const GraphicsPipelineStateDescription &description) : GraphicsPipelineState(device, description) {

    }

}