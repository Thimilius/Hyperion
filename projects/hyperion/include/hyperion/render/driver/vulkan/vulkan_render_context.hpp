//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/render/render_context.hpp"
#include "hyperion/render/driver/vulkan/vulkan_render_driver.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  class VulkanRenderContext : public IRenderContext {
  public:
    RenderContextProperties GetProperties() const override { return m_properties; }

    IRenderDriver *GetDriver() override { return &m_render_driver; }

    void Initialize(Window *main_window, const RenderContextDescriptor &descriptor) override;
    void Shutdown() override;
  private:
    RenderContextProperties m_properties;

    VulkanRenderDriver m_render_driver;
  };

}