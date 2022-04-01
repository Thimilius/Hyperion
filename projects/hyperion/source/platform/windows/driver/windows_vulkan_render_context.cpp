//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/platform/windows/driver/windows_vulkan_render_context.hpp"

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
  void WindowsVulkanRenderContext::SwapBuffers(Window *window) {

  }

  //--------------------------------------------------------------
  void WindowsVulkanRenderContext::SetVSyncMode(VSyncMode vsync_mode) {

  }

}