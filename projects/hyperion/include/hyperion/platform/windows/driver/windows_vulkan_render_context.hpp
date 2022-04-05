//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <Windows.h>

//---------------------- Project Includes ----------------------
#include "hyperion/render/driver/vulkan/vulkan_render_context.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  class WindowsVulkanRenderContext : public VulkanRenderContext {
  public:
    void Initialize(Window *main_window, const RenderContextDescriptor &descriptor) override;
    void Shutdown() override;

    void MakeCurrent(Window *window) override;
    void SwapBuffers(Window *window) override;
    void SetVSyncMode(VSyncMode vsync_mode) override;
  protected:
    Array<const char *> GetRequiredExtensions() const override;

    VkResult CreateSurface(Window *window, VkInstance instance, VkSurfaceKHR *surface) override;
  };

}