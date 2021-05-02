#pragma once

//---------------------- Library Includes ----------------------
#include <vulkan/vulkan.h>

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_render_pass.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion::Graphics {
    class VulkanGraphicsDevice;
}

namespace Hyperion::Graphics {

    class VulkanGraphicsRenderPass : public GraphicsRenderPass {
    public:
        VulkanGraphicsRenderPass(VulkanGraphicsDevice *device, const GraphicsRenderPassDescription &description);
        ~VulkanGraphicsRenderPass();
    public:
        VkRenderPass GetRenderPass() const { return m_render_pass; }
    private:
        VkRenderPass m_render_pass;
    };

}