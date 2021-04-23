//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_swap_chain.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_context.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    //--------------------------------------------------------------
    VulkanGraphicsSwapChain::VulkanGraphicsSwapChain(VulkanGraphicsContext *context, VkSurfaceKHR surface, VkQueue queue) {
        m_context = context;
        m_surface = surface;
        m_queue = queue;
    }

    //--------------------------------------------------------------
    VulkanGraphicsSwapChain::~VulkanGraphicsSwapChain() {
        vkDestroySurfaceKHR(m_context->GetInstance(), m_surface, nullptr);
    }

}