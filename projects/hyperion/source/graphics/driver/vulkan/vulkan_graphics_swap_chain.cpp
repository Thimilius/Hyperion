//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_swap_chain.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_context.hpp"
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_device.hpp"
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_utilities.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    //--------------------------------------------------------------
    VulkanGraphicsSwapChain::VulkanGraphicsSwapChain(VulkanGraphicsContext *context, VkSurfaceKHR surface, VkQueue queue) {
        m_context = context;
        m_surface = surface;
        m_queue = queue;

        QuerySwapChainSupportDetails();
        CreateSwapChain();
        CreateSwapChainImageViews();
    }

    //--------------------------------------------------------------
    VulkanGraphicsSwapChain::~VulkanGraphicsSwapChain() {
        VkDevice device = m_context->GetDevice()->GetDevice();

        for (VkImageView image_view : m_swap_chain_image_views) {
            vkDestroyImageView(device, image_view, nullptr);
        }
        vkDestroySwapchainKHR(device, m_swap_chain, nullptr);
        vkDestroySurfaceKHR(m_context->GetInstance(), m_surface, nullptr);
    }

    //--------------------------------------------------------------
    void VulkanGraphicsSwapChain::QuerySwapChainSupportDetails() {
        VkPhysicalDevice physical_device = m_context->GetPhysicalDevice();

        HYP_VULKAN_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, m_surface, &m_swap_chain_support_details.capabilities));

        uint32_t format_count;
        HYP_VULKAN_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, m_surface, &format_count, nullptr));
        if (format_count != 0) {
            m_swap_chain_support_details.formats.Resize(format_count);
            HYP_VULKAN_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, m_surface, &format_count, m_swap_chain_support_details.formats.GetData()));
        }

        uint32_t present_mode_count;
        HYP_VULKAN_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, m_surface, &present_mode_count, nullptr));
        if (present_mode_count != 0) {
            m_swap_chain_support_details.present_modes.Resize(present_mode_count);
            HYP_VULKAN_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, m_surface, &present_mode_count, m_swap_chain_support_details.present_modes.GetData()));
        }
    }

    //--------------------------------------------------------------
    void VulkanGraphicsSwapChain::CreateSwapChain() {
        VkSurfaceFormatKHR surface_format = ChooseSwapChainSurfaceFormat(m_swap_chain_support_details.formats);
        m_swap_chain_format = surface_format.format;
        VkPresentModeKHR present_mode = ChooseSwapChainPresentMode(m_swap_chain_support_details.present_modes);
        m_swap_chain_extent = ChooseSwapChainExtent(m_swap_chain_support_details.capabilities);
        uint32 image_count = m_swap_chain_support_details.capabilities.minImageCount + 1;
        if (m_swap_chain_support_details.capabilities.maxImageCount > 0 && image_count > m_swap_chain_support_details.capabilities.maxImageCount) {
            image_count = m_swap_chain_support_details.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR swap_chain_create_info = { };
        swap_chain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swap_chain_create_info.surface = m_surface;
        swap_chain_create_info.minImageCount = image_count;
        swap_chain_create_info.imageFormat = surface_format.format;
        swap_chain_create_info.imageColorSpace = surface_format.colorSpace;
        swap_chain_create_info.imageExtent = m_swap_chain_extent;
        swap_chain_create_info.imageArrayLayers = 1;
        swap_chain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        const VulkanQueueFamilyIndices queue_family_indices = m_context->GetQueueFamilyIndices();
        if (queue_family_indices.graphics_family_index != queue_family_indices.present_family_index) {
            uint32 indices[] = { queue_family_indices.graphics_family_index, queue_family_indices.present_family_index };
            swap_chain_create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            swap_chain_create_info.queueFamilyIndexCount = 2;
            swap_chain_create_info.pQueueFamilyIndices = indices;
        } else {
            swap_chain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            swap_chain_create_info.queueFamilyIndexCount = 0;
            swap_chain_create_info.pQueueFamilyIndices = nullptr;
        }
        swap_chain_create_info.preTransform = m_swap_chain_support_details.capabilities.currentTransform;
        swap_chain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swap_chain_create_info.presentMode = present_mode;
        swap_chain_create_info.clipped = VK_TRUE;
        swap_chain_create_info.oldSwapchain = VK_NULL_HANDLE;

        VkDevice device = m_context->GetDevice()->GetDevice();
        HYP_VULKAN_CHECK(vkCreateSwapchainKHR(device, &swap_chain_create_info, nullptr, &m_swap_chain));

        HYP_VULKAN_CHECK(vkGetSwapchainImagesKHR(device, m_swap_chain, &image_count, nullptr));
        m_swap_chain_images.Resize(image_count);
        HYP_VULKAN_CHECK(vkGetSwapchainImagesKHR(device, m_swap_chain, &image_count, m_swap_chain_images.GetData()));
    }

    //--------------------------------------------------------------
    void VulkanGraphicsSwapChain::CreateSwapChainImageViews() {
        m_swap_chain_image_views.Resize(m_swap_chain_images.GetLength());

        VkDevice device = m_context->GetDevice()->GetDevice();
        for (uint64 i = 0; i < m_swap_chain_images.GetLength(); i++) {
            VkImageViewCreateInfo image_view_create_info = { };
            image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            image_view_create_info.image = m_swap_chain_images[i];
            image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
            image_view_create_info.format = m_swap_chain_format;
            image_view_create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            image_view_create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            image_view_create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            image_view_create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            image_view_create_info.subresourceRange.baseMipLevel = 0;
            image_view_create_info.subresourceRange.levelCount = 1;
            image_view_create_info.subresourceRange.baseArrayLayer = 0;
            image_view_create_info.subresourceRange.layerCount = 1;

            HYP_VULKAN_CHECK(vkCreateImageView(device, &image_view_create_info, nullptr, &m_swap_chain_image_views[i]));
        }
    }

    //--------------------------------------------------------------
    VkSurfaceFormatKHR VulkanGraphicsSwapChain::ChooseSwapChainSurfaceFormat(const Array<VkSurfaceFormatKHR> &formats) {
        for (const VkSurfaceFormatKHR &format : formats) {
            if (format.format == VK_FORMAT_R8G8B8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return format;
            }
        }

        return formats[0];
    }

    //--------------------------------------------------------------
    VkPresentModeKHR VulkanGraphicsSwapChain::ChooseSwapChainPresentMode(const Array<VkPresentModeKHR> &present_modes) {
        for (const VkPresentModeKHR &present_mode : present_modes) {
            if (present_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return present_mode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    //--------------------------------------------------------------
    VkExtent2D VulkanGraphicsSwapChain::ChooseSwapChainExtent(const VkSurfaceCapabilitiesKHR &capabilities) {
        if (capabilities.currentExtent.width != UINT32_MAX) {
            return capabilities.currentExtent;
        } else {
            return VkExtent2D();
        }
    }

}