//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/platform/windows/driver/windows_vulkan_render_context.hpp"

//---------------------- Library Includes ----------------------
#include <vulkan/vulkan_win32.h>

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  //--------------------------------------------------------------
  void WindowsVulkanRenderContext::Initialize(Window *main_window, const RenderContextDescriptor &descriptor) {
    VulkanRenderContext::Initialize(main_window, descriptor);
  }

  //--------------------------------------------------------------
  void WindowsVulkanRenderContext::Shutdown() {
    VulkanRenderContext::Shutdown();
  }

  //--------------------------------------------------------------
  void WindowsVulkanRenderContext::MakeCurrent(Window *window) {

  }

  //--------------------------------------------------------------
  void WindowsVulkanRenderContext::SetVSyncMode(VSyncMode vsync_mode) {
    
  }

  //--------------------------------------------------------------
  Array<const char *> WindowsVulkanRenderContext::GetRequiredInstanceExtensions() const {
    Array<const char *> extensions = VulkanRenderContext::GetRequiredInstanceExtensions();
    extensions.Add(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
    return extensions;
  }

  //--------------------------------------------------------------
  VkResult WindowsVulkanRenderContext::CreateSurface(Window *window, VkInstance instance, VkSurfaceKHR *surface) {
    VkWin32SurfaceCreateInfoKHR create_info = { };
    create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    create_info.hinstance = GetModuleHandleW(nullptr);
    create_info.hwnd = reinterpret_cast<HWND>(window->GetNativeHandle());
    return vkCreateWin32SurfaceKHR(instance, &create_info, nullptr, surface);
  }

}