#pragma once

//---------------------- Library Includes ----------------------
#include <vulkan/vulkan.h>

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_swap_chain.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion::Graphics {
    class VulkanGraphicsContext;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    class VulkanGraphicsSwapChain : public GraphicsSwapChain {
    public:
        VulkanGraphicsSwapChain(VulkanGraphicsContext *context, VkSurfaceKHR surface, VkQueue queue);
        ~VulkanGraphicsSwapChain();
    private:
        VulkanGraphicsContext *m_context;

        VkSurfaceKHR m_surface;
        VkQueue m_queue;
    };

}