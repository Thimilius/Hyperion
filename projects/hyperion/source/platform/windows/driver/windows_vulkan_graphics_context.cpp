//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/platform/windows/driver/windows_vulkan_graphics_context.hpp"

//---------------------- Library Includes ----------------------
#include <vulkan/vulkan_win32.h>

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    //--------------------------------------------------------------
    WindowsVulkanGraphicsContext::WindowsVulkanGraphicsContext(HWND window_handle) {
        m_window_handle = window_handle;
    }

    //--------------------------------------------------------------
    VkResult WindowsVulkanGraphicsContext::CreateSurface(VkSurfaceKHR *surface) {
        VkWin32SurfaceCreateInfoKHR win32_surface_create_info = { };
        win32_surface_create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        win32_surface_create_info.hwnd = m_window_handle;
        win32_surface_create_info.hinstance = GetModuleHandle(nullptr); 

        return vkCreateWin32SurfaceKHR(m_instance, &win32_surface_create_info, nullptr, surface);
    }

    //--------------------------------------------------------------
    void WindowsVulkanGraphicsContext::CheckExtensions(Vector<const char *> &required_extension_names) {
        VulkanGraphicsContext::CheckExtensions(required_extension_names);

        CheckExtension(required_extension_names, VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
    }

}