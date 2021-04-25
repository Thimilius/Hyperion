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
        void QuerySwapChainSupportDetails();
        void CreateSwapChain();
        void CreateSwapChainImageViews();
    private:
        static VkSurfaceFormatKHR ChooseSwapChainSurfaceFormat(const Vector<VkSurfaceFormatKHR> &formats);
        static VkPresentModeKHR ChooseSwapChainPresentMode(const Vector<VkPresentModeKHR> &present_modes);
        static VkExtent2D ChooseSwapChainExtent(const VkSurfaceCapabilitiesKHR &capabilities);
    private:
        VulkanGraphicsContext *m_context;

        VkSurfaceKHR m_surface;
        VkQueue m_queue;

        VkSwapchainKHR m_swap_chain;
        Vector<VkImage> m_swap_chain_images;
        VkFormat m_swap_chain_format;
        VkExtent2D m_swap_chain_extent;
        Vector<VkImageView> m_swap_chain_image_views;

        struct SwapChainSupportDetails {
            VkSurfaceCapabilitiesKHR capabilities;

            Vector<VkSurfaceFormatKHR> formats;
            Vector<VkPresentModeKHR> present_modes;
        } m_swap_chain_support_details;
    };

}