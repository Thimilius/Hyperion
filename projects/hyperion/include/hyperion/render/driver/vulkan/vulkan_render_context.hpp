//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <vulkan/vulkan.h>

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
    void QueryLayersAndExtensions();
    void CreateInstance();
  private:
    RenderContextProperties m_properties;

    VulkanRenderDriver m_render_driver;

    Array<VkLayerProperties> m_instance_layers;
    Array<VkExtensionProperties> m_instance_extensions;

    VkInstance m_instance;
  };

}