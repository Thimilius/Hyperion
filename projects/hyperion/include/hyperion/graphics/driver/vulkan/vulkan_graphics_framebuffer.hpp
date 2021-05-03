#pragma once

//---------------------- Library Includes ----------------------
#include <vulkan/vulkan.h>

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_framebuffer.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion::Graphics {
    class VulkanGraphicsDevice;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    class VulkanGraphicsFramebuffer : public GraphicsFramebuffer {
    public:
        VulkanGraphicsFramebuffer(VulkanGraphicsDevice *device, const GraphicsFramebufferDescription &description);
    };

}