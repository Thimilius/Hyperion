//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_render_pass.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_device.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    //--------------------------------------------------------------
    VulkanGraphicsRenderPass::VulkanGraphicsRenderPass(VulkanGraphicsDevice *device, const GraphicsRenderPassDescription &description) : GraphicsRenderPass(device, description) {

    }

}