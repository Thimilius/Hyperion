//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_swap_chain.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_context.hpp"
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_utilities.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    //--------------------------------------------------------------
    VulkanGraphicsSwapChain::VulkanGraphicsSwapChain(VulkanGraphicsContext *context, VkSurfaceKHR surface, VkQueue queue) {
        m_context = context;
        m_surface = surface;
        m_queue = queue;

        QuerySwapChainSupportDetails();
    }

    //--------------------------------------------------------------
    VulkanGraphicsSwapChain::~VulkanGraphicsSwapChain() {
        vkDestroySurfaceKHR(m_context->GetInstance(), m_surface, nullptr);
    }

    //--------------------------------------------------------------
    void VulkanGraphicsSwapChain::QuerySwapChainSupportDetails() {
        VkPhysicalDevice physical_device = m_context->GetPhysicalDevice();

        HYP_VULKAN_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, m_surface, &m_swap_chain_support_details.capabilities));

        uint32_t format_count;
        HYP_VULKAN_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, m_surface, &format_count, nullptr));
        if (format_count != 0) {
            m_swap_chain_support_details.formats.resize(format_count);
            HYP_VULKAN_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, m_surface, &format_count, m_swap_chain_support_details.formats.data()));
        }

        uint32_t present_mode_count;
        HYP_VULKAN_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, m_surface, &present_mode_count, nullptr));
        if (present_mode_count != 0) {
            m_swap_chain_support_details.present_modes.resize(present_mode_count);
            HYP_VULKAN_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, m_surface, &present_mode_count, m_swap_chain_support_details.present_modes.data()));
        }
    }

}