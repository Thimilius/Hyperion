#pragma once

//---------------------- Library Includes ----------------------
#include <Windows.h>

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_context.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    class WindowsVulkanGraphicsContext : public VulkanGraphicsContext {
    public:
        WindowsVulkanGraphicsContext(HWND window_handle);
    protected:
        VkResult CreateSurface(VkSurfaceKHR *surface) override;

        void CheckExtensions(List<const char *> &required_extension_names) override;
    private:
        HWND m_window_handle;
    };

}