//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_framebuffer.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_device.hpp"
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_utilities.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    //--------------------------------------------------------------
    VulkanGraphicsFramebuffer::VulkanGraphicsFramebuffer(VulkanGraphicsDevice *device, const GraphicsFramebufferDescription &description) : GraphicsFramebuffer(device, description) {

    }

}